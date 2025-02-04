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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UInputMappingContext* MappingContext;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UInputAction* MoveAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UInputAction* LookAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UInputAction* WalkAction;

};
