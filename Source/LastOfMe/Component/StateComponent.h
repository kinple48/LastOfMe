// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTOFME_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	UStateComponent* SetIsWalking  (bool IsWalking)  { this->bIsWalking  = IsWalking;  return this; }
	UStateComponent* SetIsSprint   (bool IsSprint)   { this->bIsSprint   = IsSprint;   return this; }
	UStateComponent* SetIsCrouched (bool IsCrouched) { this->bIsCrouched = IsCrouched; return this; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "boolean")
	bool bIsWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "boolean")
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "boolean")
	bool bIsCrouched = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float CrouchSpeed = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float SprintSpeed = 800.0f;
		
};
