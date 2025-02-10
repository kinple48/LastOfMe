// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "../Character/MainPlayerCharacter.h"

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
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, logMsg);

	switch (mState)
	{
	case EEnemyState::Idle: { IdleState();  } break;
	case EEnemyState::Move: { MoveState();  } break;
	case EEnemyState::Attack: { AttackState();} break;
	case EEnemyState::Damage: { DamageState(); } break;
	case EEnemyState::Die: { DieState(); } break;
	}

}

void UEnemyFSM::IdleState()
{
}

void UEnemyFSM::MoveState()
{
	if (!target || !me) return;
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	speed = 2.0f;
	me->SetActorLocationAndRotation(FVector(me->GetActorLocation() + speed * dir * GetWorld()->DeltaTimeSeconds),FRotator(UKismetMathLibrary::MakeRotFromXZ(dir,me->GetActorUpVector())));

	if (dir.Size() <= AttackRange)
	{
		mState = EEnemyState::Attack;
	}

}

void UEnemyFSM::AttackState()
{
	speed = 0.f;
	float distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());
	if (distance > AttackRange)
	{
		mState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState()
{

}

void UEnemyFSM::DieState()
{

}

