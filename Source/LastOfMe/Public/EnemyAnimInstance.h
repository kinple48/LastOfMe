// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category=EnemyAnimInstance)
	float Speed = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=EnemyAnimInstance)
	bool Attackstate = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyAnimInstance)
	bool Bitestate = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyAnimInstance)
	bool Walkstate = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=EnemyAnimInstance)
	bool Runstate = false;

};

