// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBullet.h"

#include "RenderCore.h"
#include "Components/SphereComponent.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMyBullet::AMyBullet()
{
	bReplicates = true;
	SetReplicateMovement(true);
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);
	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	//SphereColl->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SphereColl->SetGenerateOverlapEvents(true);
	//SphereColl->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SphereColl->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->UpdatedComponent = SphereColl;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	//SphereColl->SetupAttachment(RootComponent);

	//SphereColl->OnComponentBeginOverlap.AddDynamic(this, &AMyBullet::OnPickUpSphereBeginOverlap);
}

//void AMyBullet::OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	
//}

void AMyBullet::SetData(UParticleSystem* FX, uint16 damage)
{
	TracerFX = FX;
	Damage = damage;
}

void AMyBullet::Fire_Implementation(FVector Start, FVector End, FRotator Rotation)
{
	//UParticleSystemComponent* MuzzlePSC =
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerFX, Start, Rotation, true, EPSCPoolMethod::AutoRelease);
	UParticleSystemComponent* TracerPSC =
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerFX, Start,Rotation,true,EPSCPoolMethod::AutoRelease);
	//	UGameplayStatics::SpawnEmitterAttached(TracerFX, SphereColl, NAME_None, Start, Rotation, EAttachLocation::KeepRelativeOffset, true);


	//SetActorLocation(Start);
	//SetActorRotation(Rotation);


	if (!TracerPSC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TracerPSC is null"));
		return;
	}
	
	ProjectileMovement->Velocity = (FVector::ForwardVector * ProjectileMovement->InitialSpeed);
}