// Fill out your copyright notice in the Description page of Project Settings.

#include "AIsight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "Perception/AISenseConfig_Hearing.h"

AAIsight::AAIsight()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	Sightconfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	Sightconfig->SightRadius = 500.f;
	Sightconfig->LoseSightRadius = 800.f;
	Sightconfig->PeripheralVisionAngleDegrees = 50.f;
	Sightconfig->SetMaxAge(3.f);
	Sightconfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	Sightconfig->DetectionByAffiliation.bDetectEnemies = true;
	Sightconfig->DetectionByAffiliation.bDetectNeutrals = true;
	Sightconfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*Sightconfig);
	AIPerception->SetDominantSense(Sightconfig->GetSenseImplementation());

	Damageconfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	Damageconfig->SetMaxAge(3.f);
	AIPerception->ConfigureSense(*Damageconfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 500.f;
	HearingConfig->SetMaxAge(3.f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerception->ConfigureSense(*HearingConfig);

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAIsight::PerceptionUpdated);
}
void AAIsight::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		if (!UpdatedActor->ActorHasTag(TEXT("enemy")))
		{
			if (CanSenseActor(UpdatedActor, enemyAISense::Sight) || CanSenseActor(UpdatedActor, enemyAISense::Damage) || CanSenseActor(UpdatedActor, enemyAISense::Hearing))
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("hello"));
				class AEnemy* enemy = Cast<AEnemy>(GetCharacter());
				enemy->FindComponentByClass<UEnemyFSM>()->runstate = true;
				enemy->FindComponentByClass<UEnemyFSM>()->mState = EEnemyState::Move;
			}
		}
	}
}

bool AAIsight::CanSenseActor(AActor* actor, enemyAISense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	AIPerception->GetActorsPerception(actor, ActorPerceptionBlueprintInfo);
	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case enemyAISense::Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	case enemyAISense::Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	case enemyAISense::Hearing:
		QuerySenseClass = UAISenseConfig_Hearing::StaticClass();
		break;
	}

	TSubclassOf<UAISense> LastSensedStimulusClass;

	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);
		if (QuerySenseClass == LastSensedStimulusClass)
		{
			return true;
		}
	}
	return false;
}
