// MyWeapon.cpp
#include "MyWeapon.h"
#include "MyCharacter.h"
#include "MyWeaponData.h"
#include "MyBullet.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Animation/AnimationAsset.h"

AMyWeapon::AMyWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(false);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereColl->SetupAttachment(Root);
	SphereColl->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	SphereColl->SetGenerateOverlapEvents(true);
	SphereColl->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereColl->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereColl->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnPickUpSphereBeginOverlap);
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponData && Mesh)
	{
		Mesh->SetSkeletalMesh(WeaponData->Mesh);
	}

	if (HasAuthority() && WeaponData)
	{
		BulletCount = WeaponData->BulletCount;
		ReserveAmmo = WeaponData->SwitchingBulletCount;
	}
}

void AMyWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyWeapon, BulletCount);
	DOREPLIFETIME(AMyWeapon, ReserveAmmo);
}

void AMyWeapon::OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (!MyCharacter)
		return;

	if (SphereColl)
	{
		SphereColl->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereColl->SetGenerateOverlapEvents(false);
	}

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(MyCharacter->GetSocket1P(), Rules, TEXT("1P_Equipped"));

	SetOwner(MyCharacter);
	MyCharacter->SetEquipWeapon(this);
}

// =============================
// Server-only (캐릭터 Server RPC에서 호출)
// =============================

void AMyWeapon::StartFire_Server()
{
	if (!HasAuthority())
		return;

	if (!WeaponData)
		return;

	HandleFire_Server();

	if (WeaponData->bAuto)
	{
		GetWorldTimerManager().SetTimer(
			AutoFireTimer,
			this,
			&AMyWeapon::HandleFire_Server,
			WeaponData->FireRate,
			true
		);
	}
}

void AMyWeapon::StopFire_Server()
{
	if (!HasAuthority())
		return;

	GetWorldTimerManager().ClearTimer(AutoFireTimer);
}

void AMyWeapon::HandleFire_Server()
{
	if (!HasAuthority())
		return;

	if (!WeaponData)
		return;

	if (!Mesh)
		return;

	if (BulletCount <= 0)
	{
		GetWorldTimerManager().ClearTimer(AutoFireTimer);
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return;

	AController* Controller = OwnerPawn->GetController();
	if (!Controller)
		return;

	FVector AimPoint;
	if (!BuildAimPoint_Server(AimPoint))
		return;

	const FVector MuzzleLoc = Mesh->GetSocketLocation(MuzzleName);

	FVector ShotDir = AimPoint - MuzzleLoc;
	if (ShotDir.IsNearlyZero())
	{
		ShotDir = OwnerPawn->GetActorForwardVector();
	}
	ShotDir = ShotDir.GetSafeNormal();

	TSubclassOf<AMyBullet> SpawnClass = AMyBullet::StaticClass();

	if (WeaponData->BulletClass)
	{
		SpawnClass = WeaponData->BulletClass;
	}
	else
	{
		SpawnClass = AMyBullet::StaticClass();
	}

	FActorSpawnParameters SP;
	SP.Owner = OwnerPawn;
	SP.Instigator = OwnerPawn;

	AMyBullet* Bullet = GetWorld()->SpawnActor<AMyBullet>(SpawnClass, MuzzleLoc, ShotDir.Rotation(), SP);
	if (Bullet)
	{
		// Bullet에서 데미지/헤드샷/FX(트레일/임팩트) 처리
		Bullet->InitBullet(
			WeaponData->Damage,
			WeaponData->HeadBoneName,
			WeaponData->BodyDamageType,
			WeaponData->HeadDamageType,
			WeaponData->FireTracerFX,  // TrailFX로 사용
			WeaponData->ImpactFX,
			ShotDir
		);
	}

	BulletCount--;

	if (WeaponData->FireAnim)
	{
		MulticastPlayWeaponAnim(WeaponData->FireAnim);
	}
}


void AMyWeapon::Reload_Server()
{
	if (!HasAuthority())
		return;

	if (!WeaponData)
		return;

	const int32 MaxMag = WeaponData->BulletCount;
	const int32 Need = MaxMag - BulletCount;

	if (Need <= 0)
		return;

	const int32 Load = FMath::Min(Need, ReserveAmmo);

	if (Load <= 0)
		return;

	BulletCount += Load;
	ReserveAmmo -= Load;

	MulticastPlayWeaponAnim(WeaponData->ReloadAnim);
}

// =============================
// Cosmetics
// =============================

void AMyWeapon::MulticastPlayWeaponAnim_Implementation(UAnimationAsset* Anim)
{
	if (!Mesh)
		return;

	if (!Anim)
		return;

	Mesh->PlayAnimation(Anim, false);
}

// =============================
// Helpers
// =============================

bool AMyWeapon::BuildAimPoint_Server(FVector& OutAimPoint) const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return false;

	AController* Controller = OwnerPawn->GetController();
	if (!Controller)
		return false;

	const FVector ViewLoc = OwnerPawn->GetPawnViewLocation();
	const FRotator ViewRot = OwnerPawn->GetBaseAimRotation();
	const FVector ViewDir = ViewRot.Vector();

	const FVector CamStart = ViewLoc;
	const FVector CamEnd = CamStart + ViewDir * Range;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponCamTrace), true);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerPawn);

	FHitResult CamHit;
	const bool bCamHit = GetWorld()->LineTraceSingleByChannel(CamHit, CamStart, CamEnd, ECC_Visibility, Params);

	if (bCamHit)
	{
		OutAimPoint = CamHit.ImpactPoint;
	}
	else
	{
		OutAimPoint = CamEnd;
	}

	return true;
}
