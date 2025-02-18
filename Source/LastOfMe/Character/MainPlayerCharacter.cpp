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


AMainPlayerCharacter::AMainPlayerCharacter()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f));
	springArm->TargetArmLength = 150.0f;

	playerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCam"));
	playerCam->SetupAttachment(springArm);

	MyInputCoponent = CreateDefaultSubobject<ULOMInputComponent>(TEXT("MyInputComponent"));
	StateComponent  = CreateDefaultSubobject<UStateComponent>   (TEXT("StateComponent"  ));
	
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
	
	// 만약 파일읽기가 성공했다면
	
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
	// 웨픈 베이스로 해줘야하나? 바꿔줘야하면 바꿔주기 부모로 바꿔줌 자식으로 바꾸는게 필요하면 바꾸기 
	for (auto& pari : ActionClasses)
	{
		AWeaponBase* weapon = GetWorld()->SpawnActor<AWeaponBase>(pari.Value);

		weapon->Attach(GetMesh());

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

		EnhancedInputComponent->BindAction(MyInputCoponent->IA_ChangeWeapon, ETriggerEvent::Started, this, &AMainPlayerCharacter::OnActionKey);
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
	auto anim = Cast<ULOMAnimPlayer>(GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Attack!!!!!"));
}

void AMainPlayerCharacter::TEST(const FInputActionValue& inputValue)
{
	MakeNoise(1.0f, this, GetActorLocation(), 1000.f, TEXT("enemysound"));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("enemysound!!!!!"));
}

void AMainPlayerCharacter::OnActionKey(const FInputActionValue& inputValue)
{
	FString string = inputValue.ToString();


	OnChangeActions(EActionState::REVOLVER);
	// CurActionType = EActionState::BLUNT;

	auto anim = Cast<ULOMAnimPlayer>(GetMesh()->GetAnimInstance());
	anim->EquipWeapon();
	 
	 
	 GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("OnActionKey"));
}

void AMainPlayerCharacter::OnChangeActions(EActionState InActionType)
{
	
	if (InActionType == CurActionType)
		return;

	//ChangeWeapon

	LastActionType = CurActionType;
     CurActionType =  InActionType;

	OnChangeActionEnd();
}

void AMainPlayerCharacter::StrafeOn()
{
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AMainPlayerCharacter::StrafeOff()
{
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainPlayerCharacter::OnChangeActionEnd()
{
	if (CurActionType == EActionState::UNARMED)
		StrafeOff();
	else
		StrafeOn();
}

