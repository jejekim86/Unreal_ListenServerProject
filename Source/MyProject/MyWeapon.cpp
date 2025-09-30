// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MyCharacter.h"
// Sets default values
AMyWeapon::AMyWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
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

void AMyWeapon::OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	SetActorHiddenInGame(true);
	Destroy();
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSkeletalMesh(WeaponData->Mesh);
}

void AMyWeapon::Fire()
{
	
}

void AMyWeapon::Reload()
{
}


