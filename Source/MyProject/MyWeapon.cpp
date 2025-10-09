// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MyCharacter.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AMyWeapon::AMyWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Bullet = CreateDefaultSubobject<AMyBullet>(TEXT("Bullet"));
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereColl->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereColl->SetGenerateOverlapEvents(true);
	SphereColl->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereColl->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponComponent = CreateDefaultSubobject<UMyWeaponComponent>(TEXT("WeaponComponent"));

	Mesh->SetupAttachment(Root);
	SphereColl->SetupAttachment(Root);

	SphereColl->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnPickUpSphereBeginOverlap);
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSkeletalMesh(WeaponData->Mesh);
	BulletCount = WeaponData->BulletCount;
	Bullet->SetData(WeaponData->FireTracerFX,WeaponData->FireRate, WeaponData->Damage);
}

void AMyWeapon::OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
		Mesh->AttachToComponent(MyCharacter->GetSocket1P(), rules, TEXT("1P_Equipped"));
		MyCharacter->SetEquipWeapon(this);
		SetOwner(MyCharacter);
		FlushNetDormancy();
	}
}


void AMyWeapon::Fire()
{
	if (!Mesh || !WeaponData->FireAnim) return;
	Mesh->PlayAnimation(WeaponData->FireAnim, false);
	Mesh->GetSingleNodeInstance()->SetPosition(0.f, true);
	//BulletCount--;
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->GetController())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OwnerPawn or OwnerPawn->GetController() is null"));
		return;
	}

	// 1) 카메라의 정면 방향
	FVector CamLoc;
	FRotator CamRot;
	OwnerPawn->GetController()->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector CamForward = CamRot.Vector(); // 화면 중앙(전면)	

	// 2) 머즐에서 그 방향으로 직진
	const FVector StartWS = Mesh->GetSocketLocation(MuzzleName) + CamForward * 80.f;
	const FVector EndWS = StartWS + CamForward * 15000.f; // 직선

	const FRotator ShotRot = (EndWS - StartWS).Rotation(); // 발사 방향 회전

	// 3) 코스메틱 트레이서
	FireTracer(StartWS, EndWS, ShotRot); // (Multicast RPC 권장)
}

void AMyWeapon::FireTracer_Implementation(FVector Start, FVector Impact, FRotator Rotation)
{
	UParticleSystemComponent* MuzzlePSC =
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->FireTracerFX, Start, Rotation, true, EPSCPoolMethod::AutoRelease);

	if (!MuzzlePSC) return;
	
	MuzzlePSC->SetBeamSourcePoint(0, Start, 0);
	MuzzlePSC->SetBeamTargetPoint(0, Start, 0);
}

void AMyWeapon::StartFire()
{
	if (BulletCount <= 0) return;

	switch (WeaponData->bAuto)
	{
	case true:
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AMyWeapon::Fire, WeaponData->FireRate, true);
		break;
	}
	Fire();
}

void AMyWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(AutoFireTimer);
}


void AMyWeapon::Reload()
{
	Mesh->PlayAnimation(WeaponData->ReloadAnim, false);

	if (SwitchingBulletCount - (WeaponData->BulletCount - BulletCount) < 0)
	{
		SwitchingBulletCount -= (WeaponData->BulletCount - BulletCount);
		BulletCount = WeaponData->BulletCount;
	}
}
