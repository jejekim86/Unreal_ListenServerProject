// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyWeaponComponent.h"
#include "MyWeaponData.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
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

	UMyWeaponComponent* WeaponComponent;
	USceneComponent* Root;
	uint16 BulletCount;
	uint16 SwitchingBulletCount;

	UPROPERTY(EditAnywhere)
	UMyWeaponData* WeaponData;
	UFUNCTION()
	void OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void Fire();
private:
	virtual void BeginPlay() override;
	void Reload();
};
