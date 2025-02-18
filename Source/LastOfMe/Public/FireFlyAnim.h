// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FireFlyFSM.h"
#include "FireFlyAnim.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API UFireFlyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	EFireFlyState AnimState = EFireFlyState::Idle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool bAttackPlay;
	bool AttackStart;
	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_attackStart();
};
