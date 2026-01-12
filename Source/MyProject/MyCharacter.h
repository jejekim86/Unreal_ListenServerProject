// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
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
	UPROPERTY(EDITANYWHERE)
	USceneComponent* WeaponSocket1P;
	UPROPERTY(EDITANYWHERE)
	USceneComponent* WeaponSocket3P;

	UPROPERTY(EDITANYWHERE)
	UAbilitySystemComponent* AbilitySystemComp;
	
	FORCEINLINE void SetEquipWeapon(AMyWeapon* NewWeapon) { EquipWeapon = NewWeapon; }
	FORCEINLINE AMyWeapon* GetEquipWeapon() { return EquipWeapon; }
	FORCEINLINE USceneComponent* GetSocket1P() { return WeaponSocket1P; }
	FORCEINLINE USceneComponent* GetSocket3P() { return WeaponSocket3P; }

	void StartFire();
	void StopFire();
	UFUNCTION(Server, Reliable)
	void ServerStartFire();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
		AMyWeapon* EquipWeapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
