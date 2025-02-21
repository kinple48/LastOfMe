// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle ,
	Move ,
	Attack ,
	Damage ,
	Die ,
	Bite,
	Patrol
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTOFME_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;

	UPROPERTY(VisibleAnywhere,Category = FSM)
	class AMainPlayerCharacter* target;

	UPROPERTY()
	class AEnemy* me;

	void IdleState();

	void MoveState();

	void AttackState();

	void DamageState();

	void DieState();

	void BiteState();

	void PatrolState();

	UPROPERTY(EditAnywhere, Category = FSM)
	float TraceRange = 500.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float speed = 0.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool attackstate = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bitestate = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool walkstate = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool runstate = false;

	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 2.f;
	float attackDelayTime = 2.f;
	float CurrentTime = 0.f;
	//float BiteTime = 5.0f;
	float IdleTime = 2.0f;

	UPROPERTY()
	class AAIsight* ai;
	FVector randomPos;
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	void PunchStart();
	void PunchEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 hp = 4;

	void OnDamageProcess(int32 damage);
};
