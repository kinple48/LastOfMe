// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireFlyFSM.generated.h"

UENUM(BlueprintType)
enum class EFireFlyState : uint8
{
	Idle ,
	Move ,
	RangedAttack ,
	MeleeAttack ,
	Damage ,
	Die
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTOFME_API UFireFlyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireFlyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EFireFlyState mState = EFireFlyState::Idle;

	UPROPERTY(VisibleAnywhere,Category = FSM)
	class AMainPlayerCharacter* target;

	UPROPERTY()
	class AFireFly* me;

	void IdleState();

	void MoveState();

	void RangedAttackState();

	void MeleeAttackState();

	void DamageState();

	void DieState();

	float CurrentTime = 0.f;
	float IdleDelayTime = 2.f;
	float RangedAttackRange = 600.f;
	float MeleeAttackRange = 200.f;
	float attackDelayTime = 2.f;


	UPROPERTY()
	class AFireFlyController* ai;
	FVector randomPos;
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	UPROPERTY()
	class UFireFlyAnim* Anim;

	void OnAttackEnd();
	void OnAttackStart();
};
