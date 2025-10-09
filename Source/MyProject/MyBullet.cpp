// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMyBullet::AMyBullet()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);
	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereColl->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereColl->SetGenerateOverlapEvents(true);
	SphereColl->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereColl->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	SphereColl->SetupAttachment(RootComponent);

	SphereColl->OnComponentBeginOverlap.AddDynamic(this, &AMyBullet::OnPickUpSphereBeginOverlap);
}

void AMyBullet::OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AMyBullet::SetData(UParticleSystem* FX, float speed, uint16 damage)
{
	TracerFX = FX;
	FireSpeed = speed;
	Damage = damage;
}


