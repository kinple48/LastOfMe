// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "../Character/MainPlayerCharacter.h"
#include "AIsight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spline.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "EnemyAnimInstance.h"
#include "Components/SphereComponent.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayerCharacter::StaticClass());
	if (actor)
	{
		target = Cast<AMainPlayerCharacter>(actor);
	}
	me = Cast<AEnemy>(GetOwner());

	speed = me->GetCharacterMovement()->MaxWalkSpeed;

	ai = Cast<AAIsight>(me->GetController());
}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, logMsg);

	switch (mState)
	{
	case EEnemyState::Idle: { IdleState(); } break;
	case EEnemyState::Move: { MoveState(); } break;
	case EEnemyState::Attack: { AttackState(); } break;
	case EEnemyState::Damage: { DamageState(); } break;
	case EEnemyState::Die: { DieState(); } break;
	case EEnemyState::Bite: { BiteState(); } break;
	case EEnemyState::Patrol: { PatrolState(); } break;
	}

}

void UEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if (CurrentTime >= IdleTime)
	{
		mState = EEnemyState::Patrol;
		CurrentTime = 0.f;
	}
}

void UEnemyFSM::MoveState()
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
		if (dir.Size() < AttackRange)
		{
			ai->StopMovement();
			mState = EEnemyState::Attack;
			//Anim->AnimState = mState;
			//Anim->bAttackPlay = true;
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

void UEnemyFSM::AttackState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 0;
	runstate = false;
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	FRotator newRotation = dir.Rotation();
	newRotation = UKismetMathLibrary::MakeRotFromXZ(dir, me->GetActorUpVector());
	newRotation = FMath::RInterpTo(me->GetActorRotation(), newRotation, GetWorld()->GetDeltaSeconds(), 0.2f);
	me->SetActorRotation(newRotation);


	CurrentTime += GetWorld()->GetDeltaSeconds();
	if (CurrentTime >= attackDelayTime)
	{
		attackstate = true;
		CurrentTime = 0.f;
	}


	if (dir.Size() > AttackRange)
	{
		mState = EEnemyState::Move;
		attackstate = false;
	}
}

void UEnemyFSM::DamageState()
{
	
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime >= damageDelayTime)
	{
		mState = EEnemyState::Idle;
		CurrentTime = 0.f;
	}
}

void UEnemyFSM::DieState()
{

}

void UEnemyFSM::BiteState()
{
	bitestate = true;
}

void UEnemyFSM::PatrolState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 100;
	auto result = ai->MoveToLocation(randomPos);
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
	}
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UEnemyFSM::PunchStart()
{
	me->spherecomp_r->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->isDamaged = false;
}

void UEnemyFSM::PunchEnd()
{
	me->spherecomp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->isDamaged = true;
}

void UEnemyFSM::OnDamageProcess(int32 damage)
{
	hp -= damage;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		mState = EEnemyState::Die;
	}
}
