// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "MyWeapon.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EDITANYWHERE, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* Mesh1P;
	
	UPROPERTY(EDITANYWHERE, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	float Direction;
	UPROPERTY(BlueprintReadOnly)
	bool bWeaponEquipped;
	UPROPERTY(EDITANYWHERE)
	USceneComponent* WeaponSocket1P;
	UPROPERTY(EDITANYWHERE)
	USceneComponent* WeaponSocket3P;

	AMyWeapon* EquipWeapon;

	FORCEINLINE void SetEquipWeapon(AMyWeapon* NewWeapon) { EquipWeapon = NewWeapon; }
	FORCEINLINE USceneComponent* GetSocket1P() { return WeaponSocket1P; }
	FORCEINLINE USceneComponent* GetSocket3P() { return WeaponSocket3P; }

	void StartFire();
	void StopFire();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
