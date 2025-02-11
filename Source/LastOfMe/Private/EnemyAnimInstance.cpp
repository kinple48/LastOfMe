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

}
