// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIsight.generated.h"

/**
 * 
 */
UCLASS()
class LASTOFME_API AAIsight : public AAIController
{
	GENERATED_BODY()
public:
	AAIsight();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAISenseConfig_Sight* Sightconfig;
};
