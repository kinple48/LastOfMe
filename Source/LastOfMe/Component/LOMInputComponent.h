// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LOMInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTOFME_API ULOMInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULOMInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SlowMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sprint;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Crouch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_TEST;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ChangeWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ChangeRifle;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ChangeBlunt;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ChangeKnife;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Grab; // E 

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sniper; //FF

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_FlashLight; 
};

