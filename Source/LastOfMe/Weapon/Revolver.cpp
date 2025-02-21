// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Revolver.h"
#include "BulletActor.h"
#include "../Character/MainPlayerCharacter.h"

ARevolver::ARevolver()
{
}

void ARevolver::BeginPlay()
{
	Super::BeginPlay();

}

void ARevolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARevolver::Attack()
{
	Super::Attack();

	Fire();
}

void ARevolver::Fire()
{
	//FTransform firePosition = OwnerCharacter->GetSocketTransform(TEXT("Revolver_Equip"));
	//GetWorld()->SpawnActor<ABulletActor>(BulletFactory, firePosition);
}

