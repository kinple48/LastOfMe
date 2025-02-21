// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"
#include "EnemyFSM.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	if (!enemy) return;
	FVector velocity = enemy->GetVelocity();
	FVector forwardvector = enemy->GetActorForwardVector();
	Speed = FVector::DotProduct(velocity, forwardvector);
	Attackstate = enemy->FSM->attackstate;
	Bitestate = enemy->FSM->bitestate;
	Walkstate = enemy->FSM->walkstate;
	Runstate = enemy->FSM->runstate;
}

void UEnemyAnimInstance::AnimNotify_PunchStart()
{
	if (AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner()))
	{
		enemy->FSM->PunchStart();
	}
}

void UEnemyAnimInstance::AnimNotify_PunchEnd()
{
	if (AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner()))
	{
		enemy->FSM->PunchEnd();
	}
}

