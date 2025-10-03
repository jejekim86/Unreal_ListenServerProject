// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MyCharacter.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Kismet/GameplayStatics.h"
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
		FlushNetDormancy();
	}
}


void AMyWeapon::Fire()
{
	if (!Mesh || !WeaponData->FireAnim) return;
	
	Mesh->PlayAnimation(WeaponData->FireAnim, false);
	Mesh->GetSingleNodeInstance()->SetPosition(0.f, true);
	//BulletCount--;
	FHitResult Hit;

	const FTransform MuzzleTr = Mesh->GetSocketTransform(MuzzleName, RTS_World);
	const FVector StartWS = MuzzleTr.GetLocation();
	const FVector dir = MuzzleTr.GetRotation().GetForwardVector();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s"), *dir.ToString()));
	const FVector EndWS = StartWS + dir * 10000.f;

	
	//const FVector StartWS = Mesh->GetSocketLocation(MuzzleName);
	//const FVector EndWS = Mesh->GetSocketLocation(MuzzleName);

	FireTracer(StartWS, EndWS);
}

void AMyWeapon::FireTracer_Implementation(FVector Start, FVector Impact)
{
	UParticleSystemComponent* MuzzlePSC =
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->FireTracerFX, Start);


	if (!MuzzlePSC) return;
	
	MuzzlePSC->SetBeamSourcePoint(0, Start, 0);
	MuzzlePSC->SetBeamTargetPoint(0, Impact, 0);
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
