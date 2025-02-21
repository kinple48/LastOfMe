// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"
#include "EnemyFSM.h"

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{
	if (AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner()))
	{
		enemy->FSM->OnAttackEnd();
	}
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

