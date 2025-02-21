// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFly.h"
#include "Components/SphereComponent.h"

// Sets default values
AFireFly::AFireFly()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh(), TEXT("Gun"));
	GunMeshComp->SetRelativeLocationAndRotation(FVector(-2.f,12.5f,4.f), FRotator(180.f,-90.f,30.f));
	FSM = CreateDefaultSubobject<UFireFlyFSM>(TEXT("FSM"));
	Tags.Add(FName("firefly"));

	spherecomp_r = CreateDefaultSubobject<USphereComponent>(TEXT("spherecomp_r"));
	spherecomp_r->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	spherecomp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spherecomp_r->SetSphereRadius(150.f);
	


}

// Called when the game starts or when spawned
void AFireFly::BeginPlay()
{
	Super::BeginPlay();
	spherecomp_r->OnComponentBeginOverlap.AddDynamic(this, &AFireFly::OnSphereRoverlap);
	
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

void AFireFly::OnSphereRoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isDamaged)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("damaged"));
		isDamaged = false;
	}
}

