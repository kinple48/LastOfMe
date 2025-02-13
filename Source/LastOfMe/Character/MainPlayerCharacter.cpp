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


AMainPlayerCharacter::AMainPlayerCharacter()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f));
	springArm->TargetArmLength = 150.0f;

	playerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCam"));
	playerCam->SetupAttachment(springArm);

	MyInputCoponent = CreateDefaultSubobject<ULOMInputComponent>(TEXT("MyInputComponent"));

	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
	
	// 만약 파일읽기가 성공했다면
	
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
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
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_LookUp  , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::LookUp     );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Turn    , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::Turn       );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Move    , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::Move       );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_SlowMove, ETriggerEvent::Started  , this, &AMainPlayerCharacter::SlowMove   );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Sprint  , ETriggerEvent::Ongoing  , this, &AMainPlayerCharacter::SprintStart);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Sprint  , ETriggerEvent::Completed, this, &AMainPlayerCharacter::SprintEnd  );
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Crouch  , ETriggerEvent::Started  , this, &AMainPlayerCharacter::CrouchStart);
		EnhancedInputComponent->BindAction(MyInputCoponent->IA_Crouch  , ETriggerEvent::Started  , this, &AMainPlayerCharacter::CrouchEnd  );


		EnhancedInputComponent->BindAction(MyInputCoponent->IA_TEST    , ETriggerEvent::Triggered, this, &AMainPlayerCharacter::TEST);

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


	
}

void AMainPlayerCharacter::SprintStart(const FInputActionValue& inputValue)
{
	//GetCharacterMovement()->GetMaxSpeed() = 800.0f;
}

void AMainPlayerCharacter::SprintEnd(const FInputActionValue& inputValue)
{
	//GetCharacterMovement()->GetMaxSpeed() = 400.0f;

}

void AMainPlayerCharacter::CrouchStart(const FInputActionValue& inputValue)
{
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Crouch!!!!!"));

}

void AMainPlayerCharacter::CrouchEnd(const FInputActionValue& inputValue)
{
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = false;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CrouchEnd!!!!!"));

}

void AMainPlayerCharacter::Attack(const FInputActionValue& inputValue)
{
}

void AMainPlayerCharacter::TEST(const FInputActionValue& inputValue)
{
	MakeNoise(0.5f, this, GetActorLocation(), 1000.f, TEXT("enemysound"));

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("enemysound!!!!!"));
}

