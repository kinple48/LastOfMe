// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimInstance.h"
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
	void TakeDamage(float DamageAmount);

public:

	// 빌드업 패턴? 하나씩 빌드가 가능함 자기 자신을 반환
	
	UStateComponent* SetIsWalking  (bool IsWalking)  { this->bIsWalking  = IsWalking;  return this; }
	UStateComponent* SetIsSprint   (bool IsSprint)   { this->bIsSprint   = IsSprint;   return this; }
	UStateComponent* SetIsCrouched (bool IsCrouched) { this->bIsCrouched = IsCrouched; return this; }

	UStateComponent* SetMaxHP (float   InMaxHP) { this->MaxHP  =  InMaxHP;  return this; }
	UStateComponent* SetCurHP (float   InCurHP) { this->CurHP  =  InCurHP;  return this; }
	UStateComponent* SetDamage(int32  InDamage) { this->Damage = InDamage;  return this; }

	UFUNCTION(BlueprintCallable)
	float GetCurHP();       

	UFUNCTION(BlueprintCallable)
	float GetMaxHP();       


	float GetDamage();  

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "boolean")
	bool bIsWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "boolean")
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "boolean")
	bool bIsCrouched = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "boolean")
	bool bIsAttacking = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float CrouchSpeed = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxHP = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurHP = 0;


		
};
