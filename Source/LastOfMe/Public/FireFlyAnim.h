// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FireFlyFSM.h"
#include "Animation/AnimMontage.h"
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

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_gundraw();

	UFUNCTION()
	void AnimNotify_drawgun2();

	UFUNCTION()
	void AnimNotify_GunShot();

	UFUNCTION()
	void AnimNotify_Punch_R_Start();

	UFUNCTION()
	void AnimNotify_Punch_R_End();

	UFUNCTION()
	void AnimNotify_Punch_L_Start();

	UFUNCTION()
	void AnimNotify_Punch_L_End();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	class UAnimMontage* FireFlyMontage;


};
