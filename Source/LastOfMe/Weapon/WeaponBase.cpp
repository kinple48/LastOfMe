// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Mesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("Mesh"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT(""));
	if (TempMesh.Succeeded())
	{
		GetMesh();
	}*/


}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeaponBase::Attach(USceneComponent* InParent)
{
	return AttachToComponent 
	(
		InParent,
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		HolsterSocketName 
	);
}

