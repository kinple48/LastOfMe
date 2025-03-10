// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FireFly.generated.h"

UCLASS()
class LASTOFME_API AFireFly : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFireFly();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* GunMeshComp;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UFireFlyFSM* FSM;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* spherecomp_r;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* spherecomp_l;

	UFUNCTION()
	void OnSphereRoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereLoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool isDamaged = true;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxcomp;

	UFUNCTION()
	void ongraboverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void grabendoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
