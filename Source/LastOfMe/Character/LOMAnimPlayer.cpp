// Fill out your copyright notice in the Description page of Project Settings.


#include "LOMAnimPlayer.h"
#include "MainPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/StateComponent.h"

ULOMAnimPlayer::ULOMAnimPlayer()
{
}

void ULOMAnimPlayer::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Player = Cast<AMainPlayerCharacter>(TryGetPawnOwner());
}

void ULOMAnimPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player == nullptr)
		return;
	
	velocity = Player->GetVelocity();
	forwardVector = Player->GetActorForwardVector();

	Speed = FVector::DotProduct(forwardVector, velocity);

	FVector rightVector = Player->GetActorRightVector();
	Direction = FVector::DotProduct(rightVector, velocity);
	
	FRotator BaseRotator = Player->GetActorRotation();
	CrouchDirection = CalculateDirection(velocity, BaseRotator);

	bIsCrouched = Player->bIsCrouched;	
}

void ULOMAnimPlayer::PlayAttackAnim()
{
	if(AttackAnimMontage == nullptr) return;
	Montage_Play(AttackAnimMontage);
}

void ULOMAnimPlayer::EquipWeapon()
{
	if (EquipAnimMontage == nullptr) return;
	Montage_Play(EquipAnimMontage);
}

void ULOMAnimPlayer::UnEquipWeapon()
{
	if (UnEquipAnimMontage == nullptr) return;
	Montage_Play(UnEquipAnimMontage);
}
