// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Component/LOMInputComponent.h" 
#include "../Component/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LOMAnimPlayer.h"
#include "../Weapon/WeaponBase.h"
#include "Weapon/BluntBase.h"
#include "Components/ShapeComponent.h"
#include "../Weapon/Rifle.h"
#include "Blueprint/UserWidget.h"
#include "../Weapon/Revolver.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyFSM.h"
#include "FireFly.h"
#include "FireFlyFSM.h"
#include "GameFramework/Character.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/SphereComponent.h"



AMainPlayerCharacter::AMainPlayerCharacter()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f));
	springArm->TargetArmLength = 150.0f;

	playerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCam"));
	playerCam->SetupAttachment(springArm);
	playerCam->bUsePawnControlRotation = false;


	MyInputCoponent = CreateDefaultSubobject<ULOMInputComponent>(TEXT("MyInputComponent"));
	StateComponent  = CreateDefaultSubobject<UStateComponent>   (TEXT("StateComponent"  ));


	meleeAttack_R = CreateDefaultSubobject<USphereComponent>(TEXT("meleeAttack_R"));
	meleeAttack_R->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "meleeAttack_R");
	meleeAttack_R->SetCollisionProfileName(TEXT("OverlapAll"));

	meleeAttack_L = CreateDefaultSubobject<USphereComponent>(TEXT("meleeAttack_L"));
	meleeAttack_L->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "meleeAttack_L");
	meleeAttack_L->SetCollisionProfileName(TEXT("OverlapAll"));




	
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
	
	// ���� �����бⰡ �����ߴٸ�
	
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	ConstructorHelpers::FClassFinder<ULOMAnimPlayer> TempAnimInst(TEXT("/Script/Engine.AnimBlueprint'/Game/SSA/Character/ABP_PlayerAnim.ABP_PlayerAnim_C'"));
	if (TempAnimInst.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInst.Class);
	}


}

void AMainPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		
		if (subsystem)
		{
			subsystem->AddMappingContext(MyInputCoponent->IMC_Player, 0);
		}
	}
	
	GetCharacterMovement()->MaxWalkSpeed = StateComponent->RunSpeed;

	Anim = Cast<ULOMAnimPlayer>(GetMesh()->GetAnimInstance());

	// Spawn Weapon 
	// ���� ���̽��� ������ϳ�? �ٲ�����ϸ� �ٲ��ֱ� �θ�� �ٲ��� �ڽ����� �ٲٴ°� �ʿ��ϸ� �ٲٱ� 
	FActorSpawnParameters Param;

	Param.Owner = this;

	for (auto& pari : ActionClasses)
	{
		AWeaponBase* weapon = GetWorld()->SpawnActor<AWeaponBase>(pari.Value, Param);

		weapon->AttachToHolster(GetMesh());

		ActionTypes.Add(pari.Key, weapon);
	}


	SniperUI = CreateWidget<UUserWidget>(GetWorld(), SniperUIFactory);

	_CrossHariUI = CreateWidget<UUserWidget>(GetWorld(), _CrossHariUIFactory);
	_CrossHariUI->AddToViewport();


	UAnimInstance* pAnimInst = GetMesh()->GetAnimInstance();
	if (pAnimInst != nullptr)
	{
		pAnimInst->OnPlayMontageNotifyBegin.AddDynamic(this, &AMainPlayerCharacter::HandleOnMontageNotifyBegin);
	}
}

void AMainPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_LookUp  , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::LookUp      );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Turn    , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::Turn        );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Move    , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::Move        );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_SlowMove, ETriggerEvent::Started  , this, &AMainPlayerCharacter::SlowMove    );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Sprint  , ETriggerEvent::Ongoing  , this, &AMainPlayerCharacter::SprintStart );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Sprint  , ETriggerEvent::Completed, this, &AMainPlayerCharacter::SprintEnd   );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Crouch  , ETriggerEvent::Started  , this, &AMainPlayerCharacter::CrouchStart );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Attack  , ETriggerEvent::Started  , this, &AMainPlayerCharacter::AttackAction);


		EnhancedInputComponent->BindAction(MyInputCoponent->IA_TEST    , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::TEST        );

		EnhancedInputComponent->BindAction(MyInputCoponent->IA_ChangeWeapon, ETriggerEvent::Started, this, &AMainPlayerCharacter::OnRevolverKey);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_ChangeRifle , ETriggerEvent::Started, this, &AMainPlayerCharacter::OnRifleKey);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_ChangeBlunt , ETriggerEvent::Started, this, &AMainPlayerCharacter::OnBluntKey);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_ChangeKnife , ETriggerEvent::Started, this, &AMainPlayerCharacter::OnKnifeKey);

		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Sniper, ETriggerEvent::Started  , this, &AMainPlayerCharacter::SniperAim);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Sniper, ETriggerEvent::Completed, this, &AMainPlayerCharacter::SniperAim);

		// �ؿ�s 
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Grab, ETriggerEvent::Started, this, &AMainPlayerCharacter::Grab);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_F   , ETriggerEvent::Started, this, &AMainPlayerCharacter::FKey);
	}
}

void AMainPlayerCharacter::LookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void AMainPlayerCharacter::Turn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void AMainPlayerCharacter::Move(const FInputActionValue& inputValue)
{
	FVector2D MovementVector = inputValue.Get<FVector2D>();

	/*Direction.X = MovementVector.X;
	Direction.Y = MovementVector.Y;*/

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector   RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection  , MovementVector.Y);
	}
}

void AMainPlayerCharacter::SlowMove(const FInputActionValue& inputValue)
{
	StateComponent->bIsWalking = !StateComponent->bIsWalking;

	float Speed = GetVelocity().Size();

	if (Speed > 200.0f || StateComponent->bIsWalking)
	{
		GetCharacterMovement()->MaxWalkSpeed = StateComponent->WalkSpeed;
		if (StateComponent->WalkSpeed)
		{
			MakeNoise(1.0f, this, GetActorLocation(), 1000.f, TEXT("enemysound"));
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("WalkSpeed"));
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = StateComponent->RunSpeed;
		if(StateComponent->RunSpeed)
		{
			MakeNoise(1.0f, this, GetActorLocation(), 1000.f, TEXT("enemysound"));
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("RunSpeed"));
		}
	}
}

void AMainPlayerCharacter::SprintStart()
{
	StateComponent->bIsWalking = false;

	//bIsWalking = false;

	GetCharacterMovement()->MaxWalkSpeed = StateComponent->SprintSpeed;
	if (StateComponent->SprintSpeed)
	{
		MakeNoise(1.0f, this, GetActorLocation(), 1000.f, TEXT("enemysound"));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("SprintSpeed"));
	}
}

void AMainPlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = StateComponent->RunSpeed;
}

void AMainPlayerCharacter::CrouchStart(const FInputActionValue& inputValue)
{
	if (bIsCrouched)
	{
		UnCrouch(); 
		GetCharacterMovement()->MaxWalkSpeed = StateComponent->RunSpeed;
	}
	else
	{
		Crouch();
		GetCharacterMovement()->MaxWalkSpeed = StateComponent->CrouchSpeed;
	}
}

void AMainPlayerCharacter::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingPayLoad)
{
	ComboAttackIndex--;

	if (ComboAttackIndex < 0)
	{
		UAnimInstance* pAnimInst = GetMesh()->GetAnimInstance();
		if (pAnimInst != nullptr)
		{
			pAnimInst->Montage_Stop(0.4f, Anim->AttackAnimMontage);
		}
	}

	//if (a_nNotifyName == "EnableCombo") // "EnableCombo"�� �ִϸ��̼ǿ��� ������ ��Ƽ���� �̸�
	//{
	//	bCanCombo = true; // �޺� �Է� ���� ���·� ����
	//}
	//else if (a_nNotifyName == "EndCombo") // ������ ���� ��
	//{
	//	bCanCombo = false;
	//	bIsAttacking = false; // ���� ����
	//	ComboAttackIndex = 0; // �޺� �ʱ�ȭ
	//}
}

void AMainPlayerCharacter::PerformHandSphereTraces()
{
	FVector Start = GetActorLocation() + GetActorForwardVector() * 100.0f;
	FVector End = Start + GetActorForwardVector() * 150.0f;
	float Radius = 50.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // �ڱ� �ڽ� ����

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,  // ĳ���͸� �浹 üũ
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	//if (bHit)
	//{
	//	// FireFly FSM �˻�
	//	if (auto fireflyEnemy = Cast<UFireFlyFSM>(zombiEnemy))
	//	{
	//		if (auto FenemyFSM = Cast<UFireFlyFSM>(fireflyEnemy))
	//		{
	//			FenemyFSM->OnDamageProcess(1);
	//		}
	//	}

	//	// Zombi FSM �˻�
	//	if (auto zombiEnemy = Cast<UEnemyFSM>(zombiEnemy))
	//	{
	//		if (auto ZBenemyFSM = Cast<UEnemyFSM>(zombiEnemy))
	//		{
	//			ZBenemyFSM->OnDamageProcess(1);
	//		}
	//	}
	//	
	//}
}

void AMainPlayerCharacter::AttackAction(const FInputActionValue& inputValue)
{
	if (bIsAttacking)
		return;

	switch (CurActionType)
	{
	case EActionState::UNARMED:
	{
		Anim->PlayAttackAnim(ComboAttackIndex);

		ComboAttackIndex = 1;

		FHitResult hitInfo;

		auto hitActor = hitInfo.GetActor();
		if (!hitActor)
			return;

		// FireFly FSM �˻�
		if (auto fireflyEnemy = hitActor->GetDefaultSubobjectByName(TEXT("FSM")))
		{
			if (auto FenemyFSM = Cast<UFireFlyFSM>(fireflyEnemy))
			{
				FenemyFSM->OnDamageProcess(1);
			}
		}

		// Zombi FSM �˻�
		if (auto zombiEnemy = hitActor->GetDefaultSubobjectByName(TEXT("FSM")))
		{
			if (auto ZBenemyFSM = Cast<UEnemyFSM>(zombiEnemy))
			{
				ZBenemyFSM->OnDamageProcess(1);
			}
		}

		//if (bCanCombo) // �޺� �Է��� �����ϸ� ���� ���� ����
		//{
		//	ComboAttackIndex++;
		//}
		//else // ù �����̸� �޺� ����
		//{
		//	ComboAttackIndex = 1;
		//}

		//bCanCombo = false; // �ٽ� �Է� ���� ������ ��Ȱ��ȭ
		////bIsAttacking = true;
		//Anim->PlayAttackAnim(ComboAttackIndex); // ���� �޺� �ε����� �ִϸ��̼� ����
	}
		break;
	case EActionState::REVOLVER:
	{
		ARevolver* Revolver = Cast<ARevolver>(GetCurrentAction());

		if (Revolver)
		{
			Revolver->Attack();
		}			
	}
		break;
	default:
		GetCurrentAction()->Attack();
		break;
	}
	
	/*switch (CurActionType)
	{
	case EActionState::UNARMED:
		auto anim = Cast<ULOMAnimPlayer>(GetMesh()->GetAnimInstance());
		anim->PlayAttackAnim();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Attack!!!!!"));
		break;
	case EActionState::REVOLVER:
		break;
	case EActionState::RIFLE:
		break;
	case EActionState::BLUNT:
		break;
	case EActionState::BOW:
		break;
	case EActionState::MELEE:

		break;
		default:
		break;
	}*/

	
}

void AMainPlayerCharacter::TEST(const FInputActionValue& inputValue)
{
	MakeNoise(1.0f, this, GetActorLocation(), 1000.f, TEXT("enemysound"));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("enemysound!!!!!"));
}

void AMainPlayerCharacter::OnRevolverKey(const FInputActionValue& inputValue)
{
	FString string = inputValue.ToString();


	OnChangeActions(EActionState::REVOLVER);
	// CurActionType = EActionState::BLUNT;

	//auto anim = Cast<ULOMAnimPlayer>(GetMesh()->GetAnimInstance());
	//anim->EquipWeapon();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("OnRevolverKey"));
}

void AMainPlayerCharacter::OnRifleKey(const FInputActionValue& inputValue)
{
	FString string = inputValue.ToString();


	OnChangeActions(EActionState::RIFLE);
	// CurActionType = EActionState::BLUNT;

	//auto anim = Cast<ULOMAnimPlayer>(GetMesh()->GetAnimInstance());
	//anim->EquipWeapon();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("RIFLE"));
}

void AMainPlayerCharacter::OnBluntKey(const FInputActionValue& inputValue)
{
	FString string = inputValue.ToString();

	OnChangeActions(EActionState::BLUNT);
	
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("BLUNT"));
}

void AMainPlayerCharacter::OnKnifeKey(const FInputActionValue& inputValue)
{
	FString string = inputValue.ToString();


	OnChangeActions(EActionState::KNIFE);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("OnKnifeKey"));
}

void AMainPlayerCharacter::SniperAim(const struct FInputActionValue& inputValue)
{
	if (CurActionType == EActionState::RIFLE)
	{
		bSniperAim = !bSniperAim;

		if (bSniperAim)
		{
			SniperUI->AddToViewport();
			playerCam->SetFieldOfView(45.0f);
			_CrossHariUI->RemoveFromParent();
		}
		else
		{
			SniperUI->RemoveFromParent();
			playerCam->SetFieldOfView(90.0f);
			_CrossHariUI->AddToViewport();
		}
	}

}

void AMainPlayerCharacter::OnChangeActions(EActionState InActionType)
{

	if (GetCurrentAction() != nullptr)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetCurrentAction()->GetDrawMontage()))
			return;
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetCurrentAction()->GetSheathMontage()))
			return;
	}
	
	//ChangeWeapon
	 switch (CurActionType)
	 {
	 case EActionState::UNARMED:
		  CurActionType = InActionType;
		 NextActionType = InActionType;
		 PlayAnimMontage(GetCurrentAction()->GetDrawMontage());
		 break;
	 default:
		 PlayAnimMontage(GetCurrentAction()->GetSheathMontage());

		 if (CurActionType != InActionType)
			 NextActionType = InActionType;

		 break;
	 }

	/*case EActionState::REVOLVER:
		CurActionType = InActionType;
		PlayAnimMontage(GetCurrentAction()->GetDrawMontage());
		break;
	case EActionState::RIFLE:
		break;
	case EActionState::BOW:
		break;
	case EActionState::MELEE:
		break;
	case EActionState::BLUNT:
		break;*/
}

void AMainPlayerCharacter::OnAttackBegin()
{
	StateComponent->bIsAttacking = true;

	//GetCurrentAction()->GetBodyCollider()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ���� ���� ���� 
	meleeAttack_R->SetGenerateOverlapEvents(true);
	meleeAttack_L->SetGenerateOverlapEvents(true);

}

void AMainPlayerCharacter::OnAttackEnd()
{
	StateComponent->bIsAttacking = false; 
	//GetCurrentAction()->GetBodyCollider()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ���� ���� ���� 
	meleeAttack_R->SetGenerateOverlapEvents(false);
	meleeAttack_L->SetGenerateOverlapEvents(false);
}

void AMainPlayerCharacter::OnDrawActionEnd()
{
	ActionTypes[NextActionType]->AttachToHand(GetMesh());

	NextActionType = EActionState::UNARMED;

	//if (CurActionType == EActionState::UNARMED)
	//	StrafeOff();
	//else
	//	StrafeOn();
	
}

void AMainPlayerCharacter::OnSheathActionEnd()
{
	GetCurrentAction()->AttachToHolster(GetMesh());

	if (NextActionType == EActionState::UNARMED)
	{
	    CurActionType = EActionState::UNARMED;
		//StrafeOff();
	}

	else
	{
		CurActionType = NextActionType;

		PlayAnimMontage(GetCurrentAction()->GetDrawMontage());
	}

	
}


// ��� ����
void AMainPlayerCharacter::Grab()
{
	if (cangrab)
	{
		auto controller = GetWorld()->GetFirstPlayerController();
		controller->PlayerCameraManager->StartCameraShake(CameraShake);

		GetCharacterMovement()->DisableMovement();
		this->bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("grab"));
		FString sectionName = FString::Printf(TEXT("grab"));
		this->PlayAnimMontage(Anim->GrabMontage, 1.f, FName(*sectionName));
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AEnemy::StaticClass(), FName("enemy"), FoundActors);

		float NearestDistance = MAX_FLT;
		for (AActor* Actor : FoundActors)
		{
			float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				enemy = Cast<AEnemy>(Actor);
			}
		}

		if (enemy)
		{
			FName EnemyNeckBoneName = "Neck";
			FName PlayerSocketName = "GrabSocket";

			// �÷��̾��� ���� ��ġ ��������
			FVector SocketLocation = GetMesh()->GetSocketLocation(PlayerSocketName);

			// ���� �� �� ��ġ ��������
			FVector NeckLocation = enemy->GetMesh()->GetBoneLocation(EnemyNeckBoneName);

			// ���� ��ġ ����
			FVector NewLocation = SocketLocation - (NeckLocation - enemy->GetActorLocation());

			// �÷��̾��� ���� �� ���� ���� ��������
			FVector ForwardVector = GetActorForwardVector();
			FVector RightVector = GetActorRightVector();

			// x_shift�� y_shift�� �÷��̾��� ���⿡ ���� ����
			NewLocation += ForwardVector * x_shift + RightVector * y_shift;

			enemy->SetActorLocation(NewLocation);

			// �� ȸ��
			FRotator NewRotation = GetActorRotation();
			enemy->SetActorRotation(NewRotation);
			enemy->FSM->mState = EEnemyState::Grab;
		}
	}

	if (cangrab1)
	{
		GetCharacterMovement()->DisableMovement();
		this->bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("grab"));
		FString sectionName = FString::Printf(TEXT("grab"));
		this->PlayAnimMontage(Anim->GrabMontage, 1.f, FName(*sectionName));
		TArray<AActor*> foundactors;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AFireFly::StaticClass(), FName("firefly"), foundactors);

		float NearestDistance = MAX_FLT;
		for (AActor* Actor : foundactors)
		{
			float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				firefly = Cast<AFireFly>(Actor);
			}
		}

		if (firefly)
		{
			FName EnemyNeckBoneName = "neck_01";
			FName PlayerSocketName = "GrabSocket";

			// �÷��̾��� ���� ��ġ ��������
			FVector SocketLocation = GetMesh()->GetSocketLocation(PlayerSocketName);

			// ���� �� �� ��ġ ��������
			FVector NeckLocation = firefly->GetMesh()->GetBoneLocation(EnemyNeckBoneName);

			// ���� ��ġ ����
			FVector NewLocation = SocketLocation - (NeckLocation - firefly->GetActorLocation());

			// �÷��̾��� ���� �� ���� ���� ��������
			FVector ForwardVector = GetActorForwardVector();
			FVector RightVector = GetActorRightVector();

			// x_shift�� y_shift�� �÷��̾��� ���⿡ ���� ����
			NewLocation += ForwardVector * x_shift + RightVector * y_shift;

			firefly->SetActorLocation(NewLocation);

			// �� ȸ��
			FRotator NewRotation = GetActorRotation();
			firefly->SetActorRotation(NewRotation);
			firefly->FSM->mState = EFireFlyState::Grab;
		}
	}
}

void AMainPlayerCharacter::grabend()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	this->bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainPlayerCharacter::FKey()
{
}

