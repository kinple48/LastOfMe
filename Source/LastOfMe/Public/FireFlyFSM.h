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
	Die ,
	Patrol
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

	void PatrolState();

	float CurrentTime = 0.f;
	float IdleDelayTime = 2.f;
	float RangedAttackRange = 700.f;
	UPROPERTY(EditAnywhere)
	float MeleeAttackRange = 150.f;
	float attackDelayTime = 3.f;


	UPROPERTY()
	class AFireFlyController* ai;
	FVector randomPos;
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	UPROPERTY()
	class UFireFlyAnim* Anim;

	void OnAttackEnd();
	void DrawGun();
	void DrawGun2();
	void GunShot();

	void Punch_R_Start();
	void Punch_R_End();

	void Punch_L_Start();
	void Punch_L_End();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 hp = 4;

	void OnDamageProcess(int32 damage);
};
