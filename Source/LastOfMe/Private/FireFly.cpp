// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFly.h"

// Sets default values
AFireFly::AFireFly()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh(), TEXT("Gun"));
	GunMeshComp->SetRelativeLocationAndRotation(FVector(0.f,-14.f,5.f), FRotator(0.f,-90.f,30.f));

	FSM = CreateDefaultSubobject<UFireFlyFSM>(TEXT("FSM"));

}

// Called when the game starts or when spawned
void AFireFly::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireFly::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFireFly::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

