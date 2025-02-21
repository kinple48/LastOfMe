// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "BluntBase.generated.h"

/**
 * 
 */
 class USkeletalMesh;

UCLASS()
class LASTOFME_API ABluntBase : public AWeaponBase
{
	GENERATED_BODY()



	void Attack();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Comp)
	class USphereComponent* SphereComp; 
 
};
