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

void UFireFlyAnim::AnimNotify_gundraw()
{
	
	if (AFireFly* firefly = Cast<AFireFly>(TryGetPawnOwner()))
	{
		firefly->FSM->DrawGun();
	}
}

void UFireFlyAnim::AnimNotify_drawgun2()
{
	if (AFireFly* firefly = Cast<AFireFly>(TryGetPawnOwner()))
	{
		firefly->FSM->DrawGun2();
	}
}

void UFireFlyAnim::AnimNotify_GunShot()
{
	if (AFireFly* firefly = Cast<AFireFly>(TryGetPawnOwner()))
	{
		firefly->FSM->GunShot();
	}
}

void UFireFlyAnim::AnimNotify_Punch_R_Start()
{
	if (AFireFly* firefly = Cast<AFireFly>(TryGetPawnOwner()))
	{
		firefly->FSM->Punch_R_Start();
	}
}

void UFireFlyAnim::AnimNotify_Punch_R_End()
{
	if (AFireFly* firefly = Cast<AFireFly>(TryGetPawnOwner()))
	{
		firefly->FSM->Punch_R_End();
	}
}
