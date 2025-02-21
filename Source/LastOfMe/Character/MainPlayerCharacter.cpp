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
	
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
	
	// ¸¸¾à ÆÄÀÏÀÐ±â°¡ ¼º°øÇß´Ù¸é
	
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
	// ¿þÇÂ º£ÀÌ½º·Î ÇØÁà¾ßÇÏ³ª? ¹Ù²ãÁà¾ßÇÏ¸é ¹Ù²ãÁÖ±â ºÎ¸ð·Î ¹Ù²ãÁÜ ÀÚ½ÄÀ¸·Î ¹Ù²Ù´Â°Ô ÇÊ¿äÇÏ¸é ¹Ù²Ù±â 
	FActorSpawnParameters Param;

	Param.Owner = this;

	for (auto& pari : ActionClasses)
	{
		AWeaponBase* weapon = GetWorld()->SpawnActor<AWeaponBase>(pari.Value, Param);

		weapon->AttachToHolster(GetMesh());

		ActionTypes.Add(pari.Key, weapon);
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
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_ChangeBlunt, ETriggerEvent::Started, this, &AMainPlayerCharacter::OnBluntKey);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_ChangeKnife, ETriggerEvent::Started, this, &AMainPlayerCharacter::OnKnifeKey);


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

void AMainPlayerCharacter::AttackAction(const FInputActionValue& inputValue)
{
	if (bIsAttacking)
		return;

	switch (CurActionType)
	{
	case EActionState::UNARMED:
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

	//GetCurrentAction()->GetBodyCollider()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ÃÑÀÏ ¶§´Â »©±â 
}

void AMainPlayerCharacter::OnAttackEnd()
{
	StateComponent->bIsAttacking = false; 
	//GetCurrentAction()->GetBodyCollider()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ÃÑÀÏ ¶§´Â »©±â 

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

