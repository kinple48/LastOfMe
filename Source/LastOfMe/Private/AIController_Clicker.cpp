// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Clicker.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "Perception/AISenseConfig_Hearing.h"

AAIController_Clicker::AAIController_Clicker()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

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

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAIController_Clicker::PerceptionUpdated);
}

void AAIController_Clicker::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		if (!UpdatedActor->ActorHasTag(TEXT("enemy")))
		{
			if (CanSenseActor(UpdatedActor, enemyAISenseClicker::Damage) || CanSenseActor(UpdatedActor, enemyAISenseClicker::Hearing))
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("hello"));
				class AEnemy* enemy = Cast<AEnemy>(GetCharacter());
				enemy->FindComponentByClass<UEnemyFSM>()->mState = EEnemyState::Move;
			}

		}
	}
}

bool AAIController_Clicker::CanSenseActor(AActor* actor, enemyAISenseClicker AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	AIPerception->GetActorsPerception(actor, ActorPerceptionBlueprintInfo);
	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case enemyAISenseClicker::Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	case enemyAISenseClicker::Hearing:
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
