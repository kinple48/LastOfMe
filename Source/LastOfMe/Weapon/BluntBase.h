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

public:
	ABluntBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:


	void Attack();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Comp)
	class USphereComponent* SphereComp; 
 
};
