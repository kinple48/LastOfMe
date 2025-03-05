// Fill out your copyright notice in the Description page of Project Settings.


#include "sq_runner.h"

// Sets default values
Asq_runner::Asq_runner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Asq_runner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Asq_runner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Asq_runner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

