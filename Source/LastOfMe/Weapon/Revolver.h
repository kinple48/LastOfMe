// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Revolver.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API ARevolver : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARevolver();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void Fire();

	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABulletActor> BulletFactory; 


};
