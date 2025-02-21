// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Rifle.h"
#include "Engine/StaticMesh.h"
#include "../Character/MainPlayerCharacter.h"
#include "FireFly.h"
#include "Enemy.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KismetTraceUtils.h"
#include "EnemyFSM.h"
#include "FireFlyFSM.h"

ARifle::ARifle()
{
	
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();


}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARifle::Attack()
{
	Super::Attack();

	Fire();
}

void ARifle::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fire"));

	//Super::Attack();

	FVector startPos = OwnerCharacter->playerCam->GetComponentLocation();

	FVector endPos = OwnerCharacter->playerCam->GetComponentLocation() + OwnerCharacter->playerCam->GetForwardVector() * 5000.0f;

	FHitResult hitInfo;

	FCollisionQueryParams params;

	params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);

	if (bHit == true)
	{
		FTransform BulletTrans;

		BulletTrans.SetLocation(hitInfo.ImpactPoint);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletEffectFactory, BulletTrans);

		auto hitComp = hitInfo.GetComponent();

		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			FVector dir = (endPos - startPos).GetSafeNormal();

			FVector force = dir * hitComp->GetMass() * 50000.0f;

			hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);

		}

		auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
		if (enemy)
		{
			auto enemyFSM = Cast<UEnemyFSM>(enemy);
			enemyFSM->OnDamageProcess(1);
		}

		

	}
	
	FHitResult hitResult;
	DrawDebugLineTraceSingle(GetWorld(), startPos, endPos, EDrawDebugTrace::ForDuration, true, hitResult, FLinearColor::Green, FLinearColor::Red, 3.0f);

}

