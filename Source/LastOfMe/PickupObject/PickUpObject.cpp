// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject/PickUpObject.h"
#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Character/MainPlayerCharacter.h"


// Sets default values
APickUpObject::APickUpObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh Component"));
	SetRootComponent(ItemMesh);
	// 프로젝타일 네임 바꿔주기
	ItemMesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	
	//SphereComponent->CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	//SphereComponent->SetupAttachment(RootComponent);
	//SphereComponent->SetRelativeScale3D(FVector(50.0f));
	//
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickUpObject::OnBoxBeginOverlap);
	//SphereComponent->OnComponentEndOverlap.AddDynamic  (this, &APickUpObject::OnBoxEndOverlap  );
}

// Called when the game starts or when spawned
void APickUpObject::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<AMainPlayerCharacter>(GetOwner());

	BodyCollider = Cast<UShapeComponent>(GetComponentByClass(UShapeComponent::StaticClass()));
}

// Called every frame
void APickUpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsEquip == false)
	{
		if (bIsInPlayer)
		{
			ScanPlayer();
		}
	}
}

void APickUpObject::OnAttackBegin(AMainPlayerCharacter* Player)
{
}

void APickUpObject::OnAttackEnd()
{
}

void APickUpObject::OnPickedUp()
{
	if(OwnerCharacter)
		OwnerCharacter->PlayAnimMontage(AttackMontage);
}

void APickUpObject::SetItemMesh(UStaticMesh* Mesh) const
{
	ItemMesh->SetStaticMesh(Mesh);
}

void APickUpObject::InitializeItem(const FItemBaseData& ItemData)
{
	ItemID         = ItemData.ItemID;
	ItemName       = ItemData.ItemName;
	ItemType       = ItemData.Type;
	RecoveryAmount = ItemData.RecoveryAmount;
	DamageAmount   = ItemData.DamageAmount;
	
	if (ItemData.ItemMesh)
	{
		SetItemMesh(ItemData.ItemMesh);
	}

	ItemThumbnail = ItemData.ItemThumbnail;
	Description   = ItemData.Description;
	bIsEmpty      = ItemData.bNotUse;
}

bool APickUpObject::AttachToHolster(USceneComponent* InParent)
{
	return AttachToComponent
	(
		InParent,
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		HolsterSocketName
	);
}

bool APickUpObject::AttachToHand(USceneComponent* InParent)
{
	return AttachToComponent
	(
		InParent,
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		EquipSocketName
	);
}

void APickUpObject::SetSimulate()
{
	ItemMesh->SetSimulatePhysics(true);
}

float APickUpObject::GetDamageAmount()
{
	return DamageAmount;
}

void APickUpObject::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsEquip)
	{
		return;
	}

	LOMPlayer = Cast<AMainPlayerCharacter>(OtherActor);
	if (LOMPlayer)
	{
		bIsInPlayer = true;
	}
}

void APickUpObject::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LOMPlayer == Cast<AMainPlayerCharacter>(OtherActor))
	{
		ItemMesh->SetRenderCustomDepth(false);
		bIsInPlayer = false;
		LOMPlayer = nullptr;
	}
}

void APickUpObject::ScanPlayer()
{
	if (LOMPlayer)
	{
		FVector PlayerLocation = LOMPlayer->GetActorLocation();

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), PlayerLocation, ECC_Visibility, Params);

		if (Cast<AMainPlayerCharacter>(HitResult.GetActor()))
		{
			ItemMesh->SetRenderCustomDepth(true);
		}

	}
}

void APickUpObject::bIsEquipToggle()
{
	bIsEquip = !bIsEquip;
}

bool APickUpObject::GetEquip()
{
	return bIsEquip;
}

void APickUpObject::SetRenderCustomDepthFalse()
{
	ItemMesh->SetRenderCustomDepth(false);
}

UStaticMeshComponent* APickUpObject::GetItemMeshComponent()
{
	return ItemMesh;
}

void APickUpObject::SphereComponentOverlapEventClear()
{
	SphereComponent->OnComponentBeginOverlap.Clear();
	SphereComponent->OnComponentEndOverlap.Clear();
	SphereComponent->DestroyComponent();
	SphereComponent = nullptr;
}
