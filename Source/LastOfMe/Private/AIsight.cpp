// Fill out your copyright notice in the Description page of Project Settings.

#include "AIsight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

AAIsight::AAIsight()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	Sightconfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	Sightconfig->SightRadius = 800.f;
	Sightconfig->LoseSightRadius = 1200.f;
	Sightconfig->PeripheralVisionAngleDegrees = 60.f;
	Sightconfig->SetMaxAge(3.f);
	Sightconfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	Sightconfig->DetectionByAffiliation.bDetectEnemies = true;
	Sightconfig->DetectionByAffiliation.bDetectNeutrals = true;
	Sightconfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*Sightconfig);
	AIPerception->SetDominantSense(Sightconfig->GetSenseImplementation());


}