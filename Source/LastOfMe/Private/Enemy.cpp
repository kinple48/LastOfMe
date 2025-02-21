// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	Tags.Add(FName("enemy"));

	spherecomp_r = CreateDefaultSubobject<USphereComponent>(TEXT("spherecomp_r"));
	spherecomp_r->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	spherecomp_r->SetRelativeLocation(FVector(0.f, -15.f, 0.f));
	spherecomp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spherecomp_r->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnSpherecomp_rBeginoverlap);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnSpherecomp_rBeginoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isDamaged)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("Damaged"));
	}
}



