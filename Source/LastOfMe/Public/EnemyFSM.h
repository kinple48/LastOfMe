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
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
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


	float CurrentTime = 0.f;
	float BiteTime = 5.0f;
	float IdleTime = 2.0f;

	void EnterPatrolState();

};
