// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FireFly.generated.h"

UCLASS()
class LASTOFME_API AFireFly : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFireFly();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* GunMeshComp;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UFireFlyFSM* FSM;
};
