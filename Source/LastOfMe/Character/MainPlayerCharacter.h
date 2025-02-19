// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "MainPlayerCharacter.generated.h"

/**
 * 
 */
 UENUM(BlueprintType)
	 enum class EActionState : uint8
 {
	 UNARMED ,
	 REVOLVER, 
	 RIFLE   ,
	 BOW     ,
	 MELEE   ,
	 BLUNT   ,
 };

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
	
public:
	//bool GetIsCrouched() { return bIsCrouched; }

	UFUNCTION(BlueprintCallable)
	EActionState GetActionType() { return CurActionType; }

	// 장착 해체 
	UFUNCTION(BlueprintCallable)
	void OnChangeActionEnd(); 

protected:
	void LookUp     (const FInputActionValue& inputValue);
	void Turn       (const FInputActionValue& inputValue);
	void Move       (const FInputActionValue& inputValue);
	void SlowMove   (const FInputActionValue& inputValue);

	void SprintStart();
	void SprintEnd  ();

	void CrouchStart  (const FInputActionValue& inputValue);
	void AttackAction (const FInputActionValue& inputValue);
	void TEST         (const FInputActionValue& inputValue);

	void OnActionKey(const FInputActionValue& inputValue);
	void OnChangeActions(EActionState InActionType);

	// ?? 동작을 멈추게 하는 것 
	void StrafeOn();
	void StrafeOff();

	

	AWeaponBase* GetCurrentAction()
	{
		if (ActionTypes.Contains(CurActionType))
			return ActionTypes[CurActionType];

		return nullptr; 
	}

public:
	UPROPERTY()
	class ULOMAnimPlayer* Anim;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	class ULOMInputComponent* MyInputCoponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite    , Category = "Component")
	class UStateComponent* StateComponent;


protected:


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly    , Category = "Camera")
	class UCameraComponent* playerCam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly    , Category = "Camera")
	class USpringArmComponent* springArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "ActionType")
	TMap<EActionState, class AWeaponBase*> ActionTypes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "ActionTypeClass")
	TMap<EActionState, TSubclassOf<AWeaponBase>> ActionClasses;

	EActionState  CurActionType = EActionState::UNARMED;
	EActionState NextActionType = EActionState::UNARMED;

};

