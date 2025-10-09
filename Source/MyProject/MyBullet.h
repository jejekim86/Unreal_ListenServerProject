// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBullet.generated.h"

UCLASS()
class MYPROJECT_API AMyBullet : public AActor
{
	GENERATED_BODY()
	
public:
	AMyBullet();
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement = nullptr;
	
	class UParticleSystem* TracerFX = nullptr;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereColl = nullptr;
	UFUNCTION()
	void OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	void SetData(UParticleSystem* FX, float speed, uint16 damage);
private:
	float FireSpeed;
	uint16 Damage;
	
};
