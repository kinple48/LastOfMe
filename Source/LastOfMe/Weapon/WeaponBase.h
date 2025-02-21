// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ACharacter;

UCLASS()
class LASTOFME_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	bool AttachToHolster(USceneComponent* InParent);
	bool AttachToHand   (USceneComponent* InParent);

	virtual void Attack();

	UFUNCTION()
	void OnBodyColliderBeginOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UAnimMontage*   GetDrawMontage() { return DrawMontage;   }
	UAnimMontage* GetSheathMontage() { return SheathMontage; }
	UAnimMontage* GetAttackMontage() { return AttackMontage; }

	UShapeComponent* GetBodyCollider() {return BodyCollider; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	UAnimMontage* DrawMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	UAnimMontage* SheathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AttackMontage;

protected:
	class AMainPlayerCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name) 
	FName HolsterSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
	FName EquipSocketName; 

	class AEnemy* Zombi; 

	class AFireFly* fireFly;

	class ULOMAnimPlayer* anim;

	// 블프에 만들어 놓은 것을 가져오는 것 //별론데.. // 둔기에 콜리전 씌우기 // Attack 부분 다시보기
	class UShapeComponent* BodyCollider;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> CameraShake;
};
