// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Clicker.generated.h"

UENUM()
enum class enemyAISenseClicker
{
	Damage ,
	Hearing
};

UCLASS()
class LASTOFME_API AAIController_Clicker : public AAIController
{
	GENERATED_BODY()

public:
	AAIController_Clicker();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAISenseConfig_Damage* Damageconfig;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	bool CanSenseActor(AActor* actor, enemyAISenseClicker AIPerceptionSense);
};
