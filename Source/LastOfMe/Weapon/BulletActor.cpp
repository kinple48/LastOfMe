// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WeaponBase.h"
#include "Revolver.h"
#include "Enemy.h"
#include "FireFlyFSM.h"
#include "EnemyFSM.h"
#include "../Character/MainPlayerCharacter.h"

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

	ARevolver* revolver = Cast<ARevolver>(GetOwner());

	OwnerCharacter = Cast<AMainPlayerCharacter>(GetOwner());

	//BodyCollider = Cast<UShapeComponent>(GetComponentByClass(UShapeComponent::StaticClass()));
	
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
				    , 100.0f, false );

	MovementComp->Velocity = GetActorForwardVector() * bulletSpeed;

}

void ABulletActor::OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARevolver* Revolver = Cast<ARevolver>(GetOwner());

	if (!Revolver) return;

	if (OtherActor == Revolver)
		return;

	if (OtherActor == Revolver->GetOwner())
		return;

	auto hitActor = SweepResult.GetActor();
	if (!hitActor)
		return;

	// FireFly FSM 검사
	if (auto fireflyEnemy = hitActor->GetDefaultSubobjectByName(TEXT("FSM")))
	{
		if (auto FenemyFSM = Cast<UFireFlyFSM>(fireflyEnemy))
		{
			FenemyFSM->OnDamageProcess(1);
		}
	}

	// Zombi FSM 검사
	if (auto zombiEnemy = hitActor->GetDefaultSubobjectByName(TEXT("FSM")))
	{
		if (auto ZBenemyFSM = Cast<UEnemyFSM>(zombiEnemy))
		{
			ZBenemyFSM->OnDamageProcess(1);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("BulletFire"));
}

void ABulletActor::Die()
{
	Destroy();
}

