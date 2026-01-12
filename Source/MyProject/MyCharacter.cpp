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

void AMyCharacter::StartFire()
{
	if (!IsLocallyControlled()) return;
	if (!EquipWeapon) return;

	// (선택) 로컬 코스메틱만 즉시 재생: 반응성 좋음
	// EquipWeapon->PlayLocalFireAnimOrFX();

	ServerStartFire();
}

void AMyCharacter::StopFire()
{
	if (!IsLocallyControlled()) return;
	ServerStopFire();
}

void AMyCharacter::ServerStartFire_Implementation()
{
	if (!EquipWeapon) return;

	// 서버에서만 실제 발사/타이머 시작
	EquipWeapon->StartFire_Server();
}

void AMyCharacter::ServerStopFire_Implementation()
{
	if (!EquipWeapon) return;
	EquipWeapon->StopFire_Server();
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
