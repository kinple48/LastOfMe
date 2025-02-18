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

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	// ...
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayerCharacter::StaticClass());
	if (actor)
	{
		target = Cast<AMainPlayerCharacter>(actor);
	}

	me = Cast<AEnemy>(GetOwner());

	speed = me->GetCharacterMovement()->MaxWalkSpeed;
}


// Called every frame
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
		EnterPatrolState();
		CurrentTime = 0.f;
	}
}

void UEnemyFSM::MoveState()
{
	walkstate = false;
	runstate = true;
	if (!target || !me) return;
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	FRotator newRotation = dir.Rotation();

	newRotation = UKismetMathLibrary::MakeRotFromXZ(dir,me->GetActorUpVector());

	newRotation = FMath::RInterpTo(me->GetActorRotation(), newRotation, GetWorld()->GetDeltaSeconds(), 0.2f);

	me->SetActorRotation(newRotation);

	me->AddMovementInput(dir);
	me->GetCharacterMovement()->MaxWalkSpeed = 600;

	if (dir.Size() <= AttackRange)
	{
		mState = EEnemyState::Attack;
		CurrentTime = 0.f;
	}

}

void UEnemyFSM::AttackState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 0;
	attackstate = true;
	runstate = false;
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(dir,me->GetActorUpVector()));


	CurrentTime += GetWorld()->GetDeltaSeconds();
	if (CurrentTime >= BiteTime)
	{
		mState = EEnemyState::Bite;
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

}

void UEnemyFSM::DieState()
{

}

void UEnemyFSM::BiteState()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("Bite"));
	bitestate = true;
}

void UEnemyFSM::PatrolState()
{
	if (!me || !me->GetCharacterMovement() || !me->splineactor) return;
	walkstate = true;
	me->GetCharacterMovement()->MaxWalkSpeed = 50;
	FVector destination_sp = me->splineactor->GetSplinePointasWorldPosition();
	FVector dir_sp = destination_sp - me->GetActorLocation();
	if (dir_sp.Size() < 150.0f)
	{
		me->splineactor->IncrementPatrolRoute();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("Increment"));
		EnterPatrolState();
	}
}

void UEnemyFSM::EnterPatrolState()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("Patrol"));
	if (me->splineactor->splinecomp)
	{
		if (me == nullptr || me->splineactor == nullptr || me->splineactor->splinecomp == nullptr)
		{
			return;
		}
		me->splineactor->GetSplinePointasWorldPosition();
		AAIsight* Aicontroller = Cast<AAIsight>(me->GetController());
		if (Aicontroller)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(me->splineactor->GetSplinePointasWorldPosition());
			MoveRequest.SetAcceptanceRadius(10.0f);
			Aicontroller->MoveTo(MoveRequest);
		}
	}
}