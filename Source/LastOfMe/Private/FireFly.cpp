// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFly.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "../Character/MainPlayerCharacter.h"

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

	spherecomp_l = CreateDefaultSubobject<USphereComponent>(TEXT("spherecomp_l"));
	spherecomp_l->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	spherecomp_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spherecomp_l->SetSphereRadius(150.f);

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	boxcomp->SetupAttachment(RootComponent);
	boxcomp->SetRelativeLocation(FVector(-30.f, 0.f, 0.f));
	boxcomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxcomp->SetCollisionProfileName(FName("grab"));
}

// Called when the game starts or when spawned
void AFireFly::BeginPlay()
{
	Super::BeginPlay();
	spherecomp_l->OnComponentBeginOverlap.AddDynamic(this, &AFireFly::OnSphereLoverlap);
	spherecomp_r->OnComponentBeginOverlap.AddDynamic(this, &AFireFly::OnSphereRoverlap);
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &AFireFly::ongraboverlap);
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &AFireFly::grabendoverlap);
	//Anim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
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
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("damaged_R"));
		isDamaged = false;
	}
}

void AFireFly::OnSphereLoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isDamaged)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("damaged_L"));
		isDamaged = false;
	}
}

void AFireFly::ongraboverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(OtherActor);

	if (player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("overlapped"));
		player->cangrab1 = true;
	}
}

void AFireFly::grabendoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(OtherActor);

	if (player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("End overlapped"));
		player->cangrab1 = false;
	}
}

