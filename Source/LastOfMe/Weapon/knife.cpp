// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/knife.h"
#include "Components/StaticMeshComponent.h"

Aknife::Aknife()
{
    KnifeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KnifeMesh"));
    SetRootComponent(KnifeMesh);

    ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/SSA/Asset/knife/knife1.knife1'"));

    if (TempMesh.Succeeded())
    {
        KnifeMesh->SetStaticMesh(TempMesh.Object);

       // KnifeMesh->SetRelativeScale3D(FVector(0.1f));
    }

    // 사운드 사용할 때 쓰기
    //ConstructorHelpers::FObjectFinder<USoundBase> TempSound(TEXT(""));
    //if (TempSound.Succeeded())
    //{
    //    WeaponSound = TempSound.Object;
    //}
}

void Aknife::BeginPlay()
{
    Super::BeginPlay();
}

void Aknife::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void Aknife::Attack()
{
    Super::Attack();

}
