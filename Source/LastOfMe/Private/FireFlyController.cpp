// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFlyController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "FireFlyFSM.h"
#include "FireFly.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/MainPlayerCharacter.h"
#include "FireFlyAnim.h"


AFireFlyController::AFireFlyController()
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

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AFireFlyController::PerceptionUpdated);

}

void AFireFlyController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		if (!UpdatedActor->ActorHasTag(TEXT("firefly")))
		{
			if (CanSenseActor(UpdatedActor, FireFlyAISense::Sight) || CanSenseActor(UpdatedActor, FireFlyAISense::Damage) || CanSenseActor(UpdatedActor, FireFlyAISense::Hearing))
			{
				if (!isDead)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("hello"));
					class AFireFly* firefly = Cast<AFireFly>(GetCharacter());
					firefly->FindComponentByClass<UFireFlyFSM>()->mState = EFireFlyState::Move;
					firefly->FindComponentByClass<UFireFlyFSM>()->Anim->AnimState = EFireFlyState::Move;
				}
			}
		}
	}
}

bool AFireFlyController::CanSenseActor(AActor* actor, FireFlyAISense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	AIPerception->GetActorsPerception(actor, ActorPerceptionBlueprintInfo);
	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case FireFlyAISense::Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	case FireFlyAISense::Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	case FireFlyAISense::Hearing:
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
