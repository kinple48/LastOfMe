// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FireFlyController.generated.h"

UENUM()
enum class FireFlyAISense
{
	Sight ,
	Damage ,
	Hearing
};


UCLASS()
class LASTOFME_API AFireFlyController : public AAIController
{
	GENERATED_BODY()
public:
	AFireFlyController();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAISenseConfig_Sight* Sightconfig;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAISenseConfig_Damage* Damageconfig;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	bool CanSenseActor(AActor* actor, FireFlyAISense AIPerceptionSense);

	UPROPERTY(VisibleAnywhere,Category = FSM)
	class AMainPlayerCharacter* target;

	UPROPERTY()
	class AFireFly* me;

	bool isDead = false;
};
