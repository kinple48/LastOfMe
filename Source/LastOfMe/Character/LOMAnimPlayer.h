// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerCharacter.h"
#include "LOMAnimPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API ULOMAnimPlayer : public UAnimInstance
{
	GENERATED_BODY()

	ULOMAnimPlayer();

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	void PlayAttackAnim();

	void EquipWeapon();

	void UnEquipWeapon();

	UPROPERTY(EditDefaultsOnly, Category = LOMAnimPlayer)
	class UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = LOMAnimPlayer)
	class UAnimMontage* EquipAnimMontage; 

	UPROPERTY(EditDefaultsOnly, Category = LOMAnimPlayer)
	class UAnimMontage* UnEquipAnimMontage; 

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchDirection = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector forwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMainPlayerCharacter* Player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCrouched = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActionState CurAction;

	UFUNCTION()	
	void AnimNotify_AttackEnd();


	// 안 쓰고 있음 아직 안됌 
	UFUNCTION()
	void AnimNotify_AttachRevolver(); 

};
