// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"

#include "MeshPaintVisualize.h"
#include "MyCharacter.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Camera/PlayerCameraManager.h"
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
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletClass = AMyBullet::StaticClass();

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
	//USphereComponent* PickupComp = Cast<USphereComponent>(OverlappedComponent);

	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		//TODO : 나중에 무기 교체 할떄 현재 가지고 있는 무기 없애야함
		//if (MyCharacter->GetEquipWeapon())
		//	MyCharacter->SetEquipWeapon(nullptr);


		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
		Mesh->AttachToComponent(MyCharacter->GetSocket1P(), rules, TEXT("1P_Equipped"));
		MyCharacter->SetEquipWeapon(this);
		SetOwner(MyCharacter);
		//FlushNetDormancy();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, OtherActor->GetName());
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(
				OverlappedComponent, [WeakComp = TWeakObjectPtr<UPrimitiveComponent>(OverlappedComponent)]()
				{
					WeakComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				})
		);
	}
}

void AMyWeapon::Fire_Implementation()
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
	FActorSpawnParameters SP;
	SP.Owner = OwnerPawn;
	SP.Instigator = nullptr;
	//TODO: 나중에 몬스터로 변경

	MulticastTracer(StartWS, EndWS, ShotRot);

	//AMyBullet* Bullet = GetWorld()->SpawnActor<AMyBullet>(BulletClass, StartWS, ShotRot, SP);
	//// 3) 코스메틱 트레이서
	//if (!Bullet)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bullet is null"));
	//	return;
	//}
	//Bullet->SetData(WeaponData->FireTracerFX, WeaponData->Damage);
	//Bullet->Fire(StartWS, EndWS, ShotRot);
}

void AMyWeapon::MulticastTracer_Implementation(const FVector& start, const FVector& end, const FRotator& rotation)
{

	UParticleSystemComponent* MuzzlePSC =
		UGameplayStatics::SpawnEmitterAttached(WeaponData->FireTracerFX, Mesh, MuzzleName, start, rotation,
		                                       EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None);
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->FireTracerFX, start, rotation, true, EPSCPoolMethod::AutoRelease);
	if (!MuzzlePSC) return;

	MuzzlePSC->bOwnerNoSee = true;
}


//void AMyWeapon::FireTracer_Implementation(FVector Start, FVector Impact, FRotator Rotation)
//{
//	UParticleSystemComponent* MuzzlePSC =
//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->FireTracerFX, Start, Rotation, true, EPSCPoolMethod::AutoRelease);
//	
//	if (!MuzzlePSC) return;
//	
//	MuzzlePSC->SetBeamSourcePoint(0, Start, 0);
//	MuzzlePSC->SetBeamTargetPoint(0, Start, 0);
//}


void AMyWeapon::StartFire()
{
	if (BulletCount <= 0)
	{
		StopFire();
		return;
	}

	switch (WeaponData->bAuto)
	{
	case true:
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AMyWeapon::Fire, WeaponData->FireRate, true);
		break;
	}
	//Fire();
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
