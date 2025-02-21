// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	CollisionComp->SetSphereRadius(5.0f);

	SetRootComponent(CollisionComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh (TEXT("/Script/Engine.StaticMesh'/Game/SSA/Asset/Pistol/Bullet.Bullet'"));
	
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);

		MeshComp->SetRelativeScale3D(FVector(0.1f));
	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	MovementComp->SetUpdatedComponent(CollisionComp);

	MovementComp->InitialSpeed = 5000.0f;

	MovementComp->MaxSpeed = 5000.0f;

	MovementComp->bShouldBounce = false;


}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTimerHandle deathTimer;

	GetWorld()->GetTimerManager().SetTimer(deathTimer,

	FTimerDelegate::CreateLambda( [this]()->void
				    {
				    	Destroy();
				    })
				    , 2.0f, false );
}

void ABulletActor::OnBulletBeginOverlap()
{
	
}

void ABulletActor::Die()
{
	Destroy();
}

