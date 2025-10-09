// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyWeaponComponent.h"
#include "MyWeaponData.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MyBullet.h"
#include "MyWeapon.generated.h"


UCLASS()
class MYPROJECT_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereColl;
	UPROPERTY(EditDefaultsOnly, Category="FX")
	FName MuzzleName = "MuzzleFlash";
	
	UMyWeaponComponent* WeaponComponent;
	
	UPROPERTY(VisibleAnywhere)
	AMyBullet* Bullet;
	USceneComponent* Root;
	uint16 BulletCount;
	uint16 SwitchingBulletCount;
	FTimerHandle AutoFireTimer;
	FTimerHandle TracerTimer;

	UPROPERTY(EditAnywhere)
	UMyWeaponData* WeaponData;
	UFUNCTION()
	void OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Unreliable)
	void FireTracer(FVector Start, FVector Impact, FRotator Rotation);
	//UFUNCTION(Server, Unreliable)
	void Fire();
	void StartFire();
	void StopFire();
private:
	virtual void BeginPlay() override;
	void Reload();
};
