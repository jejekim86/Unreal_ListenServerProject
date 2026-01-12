#include "MyBullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/DamageType.h"
#include "Components/SkeletalMeshComponent.h"

AMyBullet::AMyBullet()
{
	bReplicates = true;
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(6.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->OnComponentHit.AddDynamic(this, &AMyBullet::OnBulletHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Collision;
	ProjectileMovement->InitialSpeed = 4500.f;
	ProjectileMovement->MaxSpeed = 4500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 3.f;

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void AMyBullet::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* Inst = GetInstigator())
	{
		Collision->IgnoreActorWhenMoving(Inst, true);
	}

	if (AActor* OwnerActor = GetOwner())
	{
		Collision->IgnoreActorWhenMoving(OwnerActor, true);
	}

	if (TrailFX)
	{
		UGameplayStatics::SpawnEmitterAttached(TrailFX, Collision);
	}
}

void AMyBullet::InitBullet(
	float InDamage,
	FName InHeadBoneName,
	TSubclassOf<UDamageType> InBodyDamageType,
	TSubclassOf<UDamageType> InHeadDamageType,
	UParticleSystem* InTrailFX,
	UParticleSystem* InImpactFX,
	const FVector& InDir
)
{
	Damage = InDamage;
	HeadBoneName = InHeadBoneName;

	BodyDamageType = InBodyDamageType;
	HeadDamageType = InHeadDamageType;

	TrailFX = InTrailFX;
	ImpactFX = InImpactFX;

	FVector Dir = InDir.GetSafeNormal();
	ProjectileMovement->Velocity = Dir * ProjectileMovement->InitialSpeed;
	SetActorRotation(Dir.Rotation());
}

void AMyBullet::OnBulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
		return;

	if (!OtherActor)
		return;

	if (OtherActor == this)
		return;

	if (OtherActor == GetOwner())
		return;

	if (OtherActor == GetInstigator())
		return;

	FVector ShotDir = GetActorForwardVector();
	if (ProjectileMovement)
	{
		ShotDir = ProjectileMovement->Velocity.GetSafeNormal();
	}

	ApplyHitDamage(Hit, ShotDir);
	MulticastImpactFX(Hit.ImpactPoint, Hit.ImpactNormal);

	Destroy();
}

void AMyBullet::ApplyHitDamage(const FHitResult& Hit, const FVector& ShotDir)
{
	AActor* HitActor = Hit.GetActor();
	if (!HitActor)
	{
		return;
	}

	bool bHeadshot = false;

	USkeletalMeshComponent* SkelComp = Cast<USkeletalMeshComponent>(Hit.GetComponent());
	if (SkelComp)
	{
		if (HeadBoneName != NAME_None)
		{
			if (Hit.BoneName == HeadBoneName)
			{
				bHeadshot = true;
			}
		}
	}

	float FinalDamage = Damage;
	if (bHeadshot)
	{
		FinalDamage = Damage * HeadshotMultiplier;
	}

	TSubclassOf<UDamageType> DT = UDamageType::StaticClass();

	if (bHeadshot)
	{
		if (HeadDamageType)
		{
			DT = HeadDamageType;
		}
	}
	else
	{
		if (BodyDamageType)
		{
			DT = BodyDamageType;
		}
	}

	UGameplayStatics::ApplyPointDamage(
		HitActor,
		FinalDamage,
		ShotDir,
		Hit,
		GetInstigatorController(),
		this,
		DT
	);
}

void AMyBullet::MulticastImpactFX_Implementation(FVector_NetQuantize ImpactPoint,
                                                 FVector_NetQuantizeNormal ImpactNormal)
{
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, ImpactPoint, ImpactNormal.Rotation());
	}
}
