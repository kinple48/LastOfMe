// Fill out your copyright notice in the Description page of Project Settings.


#include "LOMAnimPlayer.h"
#include "MainPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	Speed = Player->GetVelocity().Size2D();

	FRotator BaseRotator = Player->GetActorRotation();

	Direction = CalculateDirection(Velocity, BaseRotator);

}
