// Fill out your copyright notice in the Description page of Project Settings.


#include "Spline.h"
#include "Components/SplineComponent.h"

// Sets default values
ASpline::ASpline()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	splinecomp = CreateDefaultSubobject<USplineComponent>(TEXT("splinecomp"));
	splinecomp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASpline::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpline::IncrementPatrolRoute()
{
	PatrollIndex = PatrollIndex + Direction;
	if (PatrollIndex == splinecomp->GetNumberOfSplinePoints() - 1)
	{
		Direction = -1;
	}
	else
	{
		if (PatrollIndex == 0)
		{
			Direction = 1;
		}
	}
}

FVector ASpline::GetSplinePointasWorldPosition()
{
	splinelocation = splinecomp->GetLocationAtSplinePoint(PatrollIndex, ESplineCoordinateSpace::World);
	return splinelocation;
}

