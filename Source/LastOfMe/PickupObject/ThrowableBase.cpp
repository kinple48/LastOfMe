// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject/ThrowableBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AThrowableBase::AThrowableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	CollisionComp->SetSphereRadius(30.0f);

	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	// 나중에 충돌처리로 바꾸기 
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/SSA/Asset/BeerGlass/Beer_Bottle.Beer_Bottle'"));

	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(0.05f));
	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	MovementComp->SetUpdatedComponent(CollisionComp);

	MovementComp->InitialSpeed = 1000.0f;

	MovementComp->MaxSpeed = 5000.0f;

	MovementComp->bShouldBounce = true;

	MovementComp->Bounciness = 0.01f;


}

// Called when the game starts or when spawned
void AThrowableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTimerHandle deathTimer; 

	GetWorld()->GetTimerManager().SetTimer(deathTimer,
	
	FTimerDelegate::CreateLambda ( [this]()->void
						{
							Destroy();
						})
						,10.0f, false);

}

void AThrowableBase::Die()
{
	Destroy();
}

