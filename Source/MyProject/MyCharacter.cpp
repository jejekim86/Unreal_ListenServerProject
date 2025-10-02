// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "MyWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	bWeaponEquipped = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(RootComponent);
	Mesh1P->SetupAttachment(Camera);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	WeaponSocket1P = CreateDefaultSubobject<USceneComponent>(TEXT("Socket1P"));
	WeaponSocket3P = CreateDefaultSubobject<USceneComponent>(TEXT("Socket3P"));
	WeaponSocket1P->SetupAttachment(Mesh1P);
	WeaponSocket3P->SetupAttachment(GetMesh());
}
float FireInterval = 3.f;
void AMyCharacter::Fire_Implementation()
{
	if (!EquipWeapon) return;
	
	EquipWeapon->Fire();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}