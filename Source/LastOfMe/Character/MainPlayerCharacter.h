// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "Components/SplineMeshComponent.h"
#include "MainPlayerCharacter.generated.h"

/**
 * 
 */
 class AWeaponBase;

 UENUM(BlueprintType)
	 enum class EActionState : uint8
 {
	 UNARMED ,
	 REVOLVER, 
	 RIFLE   ,
	 BLUNT   ,
	 KNIFE   ,
	 BOW     ,
 };

 UENUM(BlueprintType)
	 enum class FistIndex : uint8
 {
	LeftFist,
	RightFist
 };

UCLASS()
class LASTOFME_API AMainPlayerCharacter : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	AMainPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	//bool GetIsCrouched() { return bIsCrouched; }

	UFUNCTION(BlueprintCallable)
	EActionState GetActionType() { return CurActionType; }

	// 장착 동작을 하면 손에 무기를 붙일 것이다. 
	UFUNCTION(BlueprintCallable)
	void OnDrawActionEnd(); 

	// 장착 해제 무기를 장비로 집어넣는다. 
	UFUNCTION(BlueprintCallable)
	void OnSheathActionEnd();

	// 공격 중 일 때
	UFUNCTION(BlueprintCallable)
	void OnAttackBegin();

	//공격이 끝날 때 
	UFUNCTION(BlueprintCallable)
	void OnAttackEnd();

	//// 노티파이 실행
	//UPROPERTY()
	//void HandleOnMontageNotifyBegin(FName a_nNotiyfyName, const FBranchingPointNotifyPayload& a_pBranchingPayLoad);

	int ComboAttackIndex = 0;

	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingPayLoad);

	void PerformHandSphereTraces(TArray<FHitResult>& OutHits, FistIndex Fist);
	

protected:
	void LookUp     (const FInputActionValue& inputValue);
	void Turn       (const FInputActionValue& inputValue);
	void Move       (const FInputActionValue& inputValue);
	void SlowMove   (const FInputActionValue& inputValue);

	void SprintStart();
	void SprintEnd  ();

	void CrouchStart  (const FInputActionValue& inputValue);
	void AttackAction (const FInputActionValue& inputValue);
	void TEST         (const FInputActionValue& inputValue);


	void OnRevolverKey(const FInputActionValue& inputValue);
	void OnRifleKey   (const FInputActionValue& inputValue);
	void OnBluntKey   (const FInputActionValue& inputValue);
	void OnKnifeKey   (const FInputActionValue& inputValue);

	void OnFlashLight();
	
	

	void OnChangeActions(EActionState InActionType);

	void SniperAim(const struct FInputActionValue& inputValue);

	AWeaponBase* GetCurrentAction()
	{
		if (ActionTypes.Contains(CurActionType))
			return ActionTypes[CurActionType];

		return nullptr; 
	}

public:
	UPROPERTY()
	class ULOMAnimPlayer* Anim;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	class ULOMInputComponent* MyInputCoponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite    , Category = "Component")
	class UStateComponent* StateComponent;


protected:


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly    , Category = "Camera")
	class UCameraComponent* playerCam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly    , Category = "Camera")
	class USpringArmComponent* springArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "ActionType")
	TMap<EActionState, class AWeaponBase*> ActionTypes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "ActionTypeClass")
	TMap<EActionState, TSubclassOf<AWeaponBase>> ActionClasses;

	EActionState  CurActionType = EActionState::UNARMED;
	EActionState NextActionType = EActionState::UNARMED;

	

	bool bIsAttacking = false; 

	bool bSniperAim = false;

	bool bRevolverAnim = false; 

	bool bCanCombo = false; 

	bool bIsFlashLight = false; 

	UPROPERTY()
	class UUserWidget* SniperUI;

	UPROPERTY()
	class UUserWidget* _CrossHariUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> _CrossHariUIFactory;



// 잡기 구현 
public:
	void Grab();
	bool cangrab = false;
	bool cangrab1 = false;

	UPROPERTY()
	class AEnemy* enemy;

	UPROPERTY()
	class AFireFly* firefly;

	void grabend();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float x_shift = 100.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float y_shift = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* meleeAttack_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* meleeAttack_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpotLightComponent* FlashLight; 
	
	// 스플라인메쉬 
public:
	// 스플라인메쉬가 시작 되는 곳 나중에 핸드 소켓 만들어서 바꿔주기 
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spline")
	UArrowComponent* ThrowLocation; 

	// SplineMesh 에 붙이고 끝 부분에 붙일 데칼을 생성해줌
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	class UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	class UDecalComponent* CircleDecal; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "ture"))
	class USplineComponent* Spline_Path;

	TArray<USplineMeshComponent*> Spline_Mesh; 

	void UpdateSplinePath();

	// 던지는 
	FVector ThrowDirection;

	//int32 SplineCount = 0;

	TArray<AActor*> IgnoreActors;

	void Throw(const FInputActionValue& inputValue);

	// 일단 이렇게 만들고 바꿔줄거임 ㅜㅜ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AThrowableBase> ThrowFactory;

	bool bIsThrow;
};

