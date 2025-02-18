// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFlyAnim.h"
#include "FireFly.h"

void UFireFlyAnim::AnimNotify_AttackEnd()
{
	if (AFireFly* firefly = Cast<AFireFly>(TryGetPawnOwner()))
	{
		firefly->FSM->OnAttackEnd();
	}
}

void UFireFlyAnim::AnimNotify_attackStart()
{
	if (AFireFly* firefly = Cast<AFireFly>(TryGetPawnOwner()))
	{
		firefly->FSM->OnAttackStart();
	}
}
