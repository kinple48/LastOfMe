// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "MainPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API AMainPlayerCharacter : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	AMainPlayerCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	class ULOMInputComponent* MyInputCoponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* playerCam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* springArm;


};
