// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "knife.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API Aknife : public AWeaponBase
{
	GENERATED_BODY()

public:
	Aknife();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:


	void Attack();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* KnifeMesh;

	


	//UPROPERTY(EditAnywhere, Category = BulletEffect)
	//class UParticleSystem* KnifeEffectFactory;
	
};
