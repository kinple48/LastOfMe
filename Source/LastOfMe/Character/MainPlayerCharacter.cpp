// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../LOMInputComponent.h"

AMainPlayerCharacter::AMainPlayerCharacter()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f));
	springArm->TargetArmLength = 150.0f;

	playerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCam"));
	playerCam->SetupAttachment(springArm);

	MyInputCoponent = CreateDefaultSubobject<ULOMInputComponent>(TEXT("MyInputComponent"));

	// 캐릭터 메시 넣어주기 
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/ Script / Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	//
	//// 만약 파일읽기가 성공했다면
	//
	//if (TempMesh.Succeeded())
	//{
	//	//로드한 메시를 넣어주고 싶다.
	//	GetMesh()->SetSkeletalMesh(TempMesh.Object);
	//
	//	// 위치값과 회전값을 반영해주고 싶다.
	//	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	//}
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
		EnhancedInputComponent->BindAction(MyInputCoponent->MoveAction, ETriggerEvent::Started, this, &AMainPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MyInputCoponent->LookAction, ETriggerEvent::Started, this, &AMainPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MyInputCoponent->WalkAction, ETriggerEvent::Started, this, &AMainPlayerCharacter::Walk);

	}
}

void AMainPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector   RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(  RightDirection, MovementVector.X);

	}
}

void AMainPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput  (LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainPlayerCharacter::Walk(const FInputActionValue& Value)
{
	
}
