// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Revolver.h"
#include "BulletActor.h"
#include "../Character/MainPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "EnemyFSM.h"
#include "FireFlyFSM.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KismetTraceUtils.h"
#include "GameFramework/Actor.h"

ARevolver::ARevolver()
{
    RevolverMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    SetRootComponent(RevolverMesh);

	//FirePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePositon"));
	//FirePosition->SetupAttachment(RevolverMesh);
	//FirePosition->SetRelativeLocationAndRotation(FVector(385.0f, -170.0f, 60.0f), FRotator(0.0f));

    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SSA/Asset/Revolver/Source/Smith___Wesson_s_629_Classic_SketchFab.Smith___Wesson_s_629_Classic_SketchFab'"));

    if (TempMesh.Succeeded())
    {
        RevolverMesh->SetSkeletalMesh(TempMesh.Object);
    }

    ConstructorHelpers::FObjectFinder<USoundBase> TempSound (TEXT("/Script/Engine.SoundWave'/Game/SSA/UI/Rifle.Rifle'"));
	if (TempSound.Succeeded())
	{
		WeaponSound = TempSound.Object;
	}

}

void ARevolver::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<AMainPlayerCharacter>(GetOwner());

}

void ARevolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARevolver::Attack()
{
	Super::Attack();

	UGameplayStatics::PlaySound2D(GetWorld(), WeaponSound);

	Fire();
}

void ARevolver::Fire()
{
    //if (!BulletFactory || !OwnerCharacter) return;  // 총알 팩토리 또는 캐릭터가 없으면 리턴


	/*FTransform FirePos = FirePosition->GetComponentTransform();
	GetWorld()->SpawnActor<ABulletActor>(BulletFactory, FirePos);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Bullet"));*/

    //FVector FireLocation = FirePosition->GetComponentLocation();
    //FRotator FireRotation = FirePosition->GetComponentRotation();
    //FTransform BulletTransform(FireRotation, FireLocation);

    //GetWorld()->SpawnActor<ABulletActor>(BulletFactory, BulletTransform);

    /*FTransform firePosiotin = RevolverMesh->GetSocketTransform(TEXT("FirePosition"));
    GetWorld()->SpawnActor<ABulletActor>(BulletFactory, firePosiotin);

    if (!BulletFactory || !OwnerCharacter)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("BulletFactory or OwnerCharacter is NULL!"));
        return;
    }*/

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

		//이펙트 사용할 때 켜주기
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletEffectFactory, BulletTrans);

		auto hitComp = hitInfo.GetComponent();

		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			FVector dir = (endPos - startPos).GetSafeNormal();

			FVector force = dir * hitComp->GetMass() * 50000.0f;

			hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
		}

		auto hitActor = hitInfo.GetActor();
		if (!hitActor)
			return;

		// FireFly FSM 검사
		if (auto fireflyEnemy = hitActor->GetDefaultSubobjectByName(TEXT("FSM")))
		{
			if (auto FenemyFSM = Cast<UFireFlyFSM>(fireflyEnemy))
			{
				FenemyFSM->OnDamageProcess(1);
			}
		}

		// Zombi FSM 검사
		if (auto zombiEnemy = hitActor->GetDefaultSubobjectByName(TEXT("FSM")))
		{
			if (auto ZBenemyFSM = Cast<UEnemyFSM>(zombiEnemy))
			{
				ZBenemyFSM->OnDamageProcess(1);
			}
		}


	}

	FHitResult hitResult;
	DrawDebugLineTraceSingle(GetWorld(), startPos, endPos, EDrawDebugTrace::ForDuration, true, hitResult, FLinearColor::Green, FLinearColor::Red, 3.0f);

}

