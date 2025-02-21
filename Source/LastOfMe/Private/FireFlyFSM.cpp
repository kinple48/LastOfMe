// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFlyFSM.h"
#include "../Character/MainPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "FireFly.h"
#include "FireFlyController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FireFlyAnim.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "KismetTraceUtils.h"
#include "CollisionQueryParams.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UFireFlyFSM::UFireFlyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFireFlyFSM::BeginPlay()
{
	Super::BeginPlay();
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayerCharacter::StaticClass());
	if (actor)
	{
		target = Cast<AMainPlayerCharacter>(actor);
	}
	me = Cast<AFireFly>(GetOwner());

	ai = Cast<AFireFlyController>(me->GetController());
	if (me)
	{
		Anim = Cast<UFireFlyAnim>(me->GetMesh()->GetAnimInstance());
	}

}


// Called every frame
void UFireFlyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, logMsg);

	switch (mState)
	{
	case EFireFlyState::Idle: { IdleState(); } break;
	case EFireFlyState::Move: { MoveState(); } break;
	case EFireFlyState::RangedAttack: { RangedAttackState(); } break;
	case EFireFlyState::MeleeAttack: { MeleeAttackState(); } break;
	case EFireFlyState::Damage: { DamageState(); } break;
	case EFireFlyState::Die: { DieState(); } break;
	case EFireFlyState::Patrol: { PatrolState(); } break;
	}
}

void UFireFlyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime >= IdleDelayTime)
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
		//mState = EFireFlyState::Move;
		mState = EFireFlyState::Patrol;
		CurrentTime = 0.f;
		Anim->AnimState = mState;
	}
}

void UFireFlyFSM::MoveState()
{
	if (!target || !me) return;
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	FRotator newRotation = dir.Rotation();
	newRotation = UKismetMathLibrary::MakeRotFromXZ(dir, me->GetActorUpVector());
	newRotation = FMath::RInterpTo(me->GetActorRotation(), newRotation, GetWorld()->GetDeltaSeconds(), 1.5f);
	me->SetActorRotation(newRotation);

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;

	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	ai->BuildPathfindingQuery(req, query);
	FPathFindingResult r = ns->FindPathSync(query);

	if (r.Result == ENavigationQueryResult::Success)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 600;
		ai->MoveToLocation(destination);
		if (dir.Size() < RangedAttackRange)
		{
			ai->StopMovement();
			mState = EFireFlyState::RangedAttack;
			Anim->AnimState = mState;
			Anim->bAttackPlay = true;
			CurrentTime = attackDelayTime;
		}
	}
	else
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 100;
		auto result = ai->MoveToLocation(randomPos);
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
		}
	}

}

void UFireFlyFSM::RangedAttackState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();


	FRotator newRotation = dir.Rotation();
	newRotation = UKismetMathLibrary::MakeRotFromXZ(dir, me->GetActorUpVector());
	newRotation = FMath::RInterpTo(me->GetActorRotation(), newRotation, GetWorld()->GetDeltaSeconds(), 1.5f);
	me->SetActorRotation(newRotation);

	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime >= attackDelayTime)
	{
		CurrentTime = 0.f;
		Anim->bAttackPlay = true;
	}

	float distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());

	if (distance <= MeleeAttackRange)
	{
		mState = EFireFlyState::MeleeAttack;
		Anim->AnimState = mState;
		Anim->bAttackPlay = true;
		CurrentTime = attackDelayTime;
	}

	if (distance > RangedAttackRange)
	{
		mState = EFireFlyState::Move;
		Anim->AnimState = mState;
		CurrentTime = 0;
	}
}

void UFireFlyFSM::MeleeAttackState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	FRotator newRotation = dir.Rotation();
	newRotation = UKismetMathLibrary::MakeRotFromXZ(dir, me->GetActorUpVector());
	newRotation = FMath::RInterpTo(me->GetActorRotation(), newRotation, GetWorld()->GetDeltaSeconds(), 1.5f);
	me->SetActorRotation(newRotation);

	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime >= attackDelayTime)
	{
		CurrentTime = 0.f;
		Anim->bAttackPlay = true;
	}

	float distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());

	if (distance > MeleeAttackRange)
	{
		mState = EFireFlyState::RangedAttack;
		Anim->AnimState = mState;
		Anim->bAttackPlay = true;
		CurrentTime = attackDelayTime;
	}
}

void UFireFlyFSM::DamageState()
{

}

void UFireFlyFSM::DieState()
{

}


void UFireFlyFSM::PatrolState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 100;
	auto result = ai->MoveToLocation(randomPos);
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
	}
}

bool UFireFlyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UFireFlyFSM::OnAttackEnd()
{
	Anim->bAttackPlay = false;
}

void UFireFlyFSM::DrawGun()
{
	me->GunMeshComp->AttachToComponent(me->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale ,TEXT("Gun2"));
	me->GunMeshComp->SetRelativeTransform(FTransform(FRotator(180.f, 90.f, 180.f), FVector(-14.f, 6.f, 6.f)));
	me->GunMeshComp->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
}

void UFireFlyFSM::DrawGun2()
{
	me->GunMeshComp->AttachToComponent(me->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Gun"));
	me->GunMeshComp->SetRelativeTransform(FTransform(FRotator(180.f, -90.f, 30.f), FVector(-2.f, 12.5f, 4.f)));
	me->GunMeshComp->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
}

void UFireFlyFSM::GunShot()
{
	FVector startPos = me->GunMeshComp->GetComponentLocation();
	float accuracy = 0.8;
	FVector gundir = (target->GetActorLocation() - me->GunMeshComp->GetComponentLocation()).GetSafeNormal();
	FVector finaldir = FMath::Lerp(me->GunMeshComp->GetRightVector()*(- 1),gundir,accuracy);
	FVector endPos = startPos + finaldir * 3000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(me);
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	if (bHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("On Hit"));
	}

	FHitResult hitResult;
	DrawDebugLineTraceSingle(GetWorld(), startPos, endPos, EDrawDebugTrace::ForDuration, true, hitResult, FLinearColor::Green, FLinearColor::Red, 3.f);
}

void UFireFlyFSM::Punch_R_Start()
{
	me->spherecomp_r->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	me->isDamaged = false;
}

void UFireFlyFSM::Punch_R_End()
{
	me->spherecomp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->isDamaged = true;
}

void UFireFlyFSM::OnDamageProcess(int32 damage)
{
	hp -= damage;
	if (hp > 0)
	{
		mState = EFireFlyState::Damage;
	}
	else
	{
		mState = EFireFlyState::Die;
	}
}

