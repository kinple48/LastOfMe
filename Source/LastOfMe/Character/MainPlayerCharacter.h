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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	void Move(const FInputActionValue&   inputValue);
	void LookUp(const FInputActionValue& inputValue);
	void Turn(const FInputActionValue&   inputValue);
	void Sprint(const FInputActionValue& inputValue);
	void Attack(const FInputActionValue& inputValue);



protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	class ULOMInputComponent* MyInputCoponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* playerCam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* springArm;

	FVector Direction;

};

