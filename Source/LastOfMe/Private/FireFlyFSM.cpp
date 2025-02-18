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
	}
}

void UFireFlyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime >= IdleDelayTime)
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
		mState = EFireFlyState::Move;
		CurrentTime = 0.f;
		Anim->AnimState = mState;
	}
}

void UFireFlyFSM::MoveState()
{
	if (!target || !me) return;
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

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
		if (dir.Size() <= RangedAttackRange)
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
	}

	if (distance > RangedAttackRange)
	{
		mState = EFireFlyState::Move;
		Anim->AnimState = mState;
	}
}

void UFireFlyFSM::MeleeAttackState()
{

}

void UFireFlyFSM::DamageState()
{

}

void UFireFlyFSM::DieState()
{

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

void UFireFlyFSM::OnAttackStart()
{
	Anim->AttackStart = true;
}

