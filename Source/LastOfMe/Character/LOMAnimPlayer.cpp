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

	CurAction = Player->GetActionType();
}

void ULOMAnimPlayer::PlayAttackAnim(int32 ComboIndex)
{
	if(AttackAnimMontage == nullptr) return;
	Montage_Play(AttackAnimMontage);

	//if (AttackAnimMontage == nullptr) return;

	//FName SectionName = *FString::Printf(TEXT("Combo%d"), ComboIndex); // "Combo1", "Combo2" 등으로 변환

	//if (Montage_IsPlaying(AttackAnimMontage))
	//{
	//	Montage_JumpToSection(SectionName, AttackAnimMontage);
	//}
	//else
	//{
	//	Montage_Play(AttackAnimMontage);
	//	Montage_JumpToSection(SectionName, AttackAnimMontage);
	//}
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

void ULOMAnimPlayer::AnimNotify_grabend()
{
	if (AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(TryGetPawnOwner()))
	{
		player->grabend();
	}
}

void ULOMAnimPlayer::AnimNotify_AttackEnd()
{
	Player = Cast<AMainPlayerCharacter>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->Anim->AnimNotify_AttackEnd();
	}
}

void ULOMAnimPlayer::AnimNotify_AttachRevolver()
{
	Player = Cast<AMainPlayerCharacter>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		//Player->Anim->AttachRevolver();
		Player->OnDrawActionEnd();
	}
}
