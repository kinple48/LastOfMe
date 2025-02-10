// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIsight.generated.h"

UENUM()
enum class enemyAISense
{
	Sight UMETA(DisplayName = "시야감지"),
	Damage UMETA(DisplayName = "피격감지")
};


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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAISenseConfig_Damage* Damageconfig;

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	bool CanSenseActor(AActor* actor, enemyAISense AIPerceptionSense);
};
