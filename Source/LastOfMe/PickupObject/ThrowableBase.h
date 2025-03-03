// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableBase.generated.h"

UCLASS()
class LASTOFME_API AThrowableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name) 
	FName EquipName;

	void Die();

};
