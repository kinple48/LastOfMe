// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API ARifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARifle();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override; 

public:
	void Fire();

	void Attack();

public:

	//UPROPERTY(EditAnywhere, Category = BulletEffect)
	//class UParticleSystem* BulletEffectFactory;


};
