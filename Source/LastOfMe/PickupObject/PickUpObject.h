// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpObject.generated.h"

class AMainPlayerCharacter;
class USphereComponent;
class APickUpObject;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None        UMETA(DisPlayName = "None"     ),
	BluntBase   UMETA(DisPlayName = "BluntBase"),
	Revolver    UMETA(DisPlayName = "Revolver" ),
	BeerGlass   UMETA(DisPlayName = "BeerGlass"),
};

USTRUCT(BlueprintType)
struct FItemBaseData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 ItemID = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName = TEXT("Unknown Item");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType Type = EItemType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float RecoveryAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float DamageAmount = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UStaticMesh* ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSoftObjectPtr<UTexture2D> ItemThumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText Description = FText::FromString(TEXT("No Description"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bNotUse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<APickUpObject> ItemClass = nullptr;

	FItemBaseData()
		: ItemID(-1)
		, ItemName(TEXT("Unknown Item"))
		, Type(EItemType::None)
		, RecoveryAmount(0.0f)
		, DamageAmount(0.0f)
		, ItemMesh(nullptr)
		, ItemThumbnail(nullptr)
		, Description(FText::FromString(TEXT("No Description")))
		, bNotUse(false)
		, ItemClass(nullptr)
	{
	}
};

UCLASS()
class LASTOFME_API APickUpObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	int32 ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	float RecoveryAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	float DamageAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> ItemThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(AllowPrivateAccess))
	bool bIsEmpty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName HolsterSocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EquipSocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* DrawMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* SheathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UShapeComponent* BodyCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Component")
	USphereComponent* SphereComponent;

public:

	virtual void OnAttackBegin(AMainPlayerCharacter* Player);
	virtual void OnAttackEnd();

	virtual void OnPickedUp();

	void SetItemMesh(UStaticMesh* Mesh) const;

	virtual void InitializeItem(const FItemBaseData& ItemData);

	bool AttachToHolster(USceneComponent* InParent);
	bool AttachToHand   (USceneComponent* InParent);

	UAnimMontage* GetDrawMontage()   { return DrawMontage; }
	UAnimMontage* GetSheathMontage() { return SheathMontage; }

	class AMainPlayerCharacter* OwnerCharacter;

	UShapeComponent* GetBodyCollider() { return BodyCollider; }

	void SetSimulate();

	float GetDamageAmount();

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	bool bIsInPlayer;

	void ScanPlayer();

protected:
	bool bIsEquip;
	
	UPROPERTY()
	AMainPlayerCharacter* LOMPlayer;
	
public:
	UFUNCTION(BlueprintCallable)
	void bIsEquipToggle();

	bool GetEquip();

	void SetRenderCustomDepthFalse();

	UStaticMeshComponent* GetItemMeshComponent();
	
	void SphereComponentOverlapEventClear();

};
