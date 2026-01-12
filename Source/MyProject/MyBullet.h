#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UDamageType;

UCLASS()
class MYPROJECT_API AMyBullet : public AActor
{
	GENERATED_BODY()

public:
	AMyBullet();

	// Weapon에서 스폰 직후 서버에서 세팅
	void InitBullet(
		float InDamage,
		FName InHeadBoneName,
		TSubclassOf<UDamageType> InBodyDamageType,
		TSubclassOf<UDamageType> InHeadDamageType,
		UParticleSystem* InTrailFX,
		UParticleSystem* InImpactFX,
		const FVector& InDir
	);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collision = nullptr;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	// 데미지는 서버만 쓰면 되므로 Replicate 안 해도 됨
	float Damage = 20.f;
	float HeadshotMultiplier = 2.f;
	FName HeadBoneName = TEXT("head");

	TSubclassOf<UDamageType> BodyDamageType;
	TSubclassOf<UDamageType> HeadDamageType;

	UPROPERTY()
	UParticleSystem* TrailFX = nullptr;

	UPROPERTY()
	UParticleSystem* ImpactFX = nullptr;

	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	                 UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ApplyHitDamage(const FHitResult& Hit, const FVector& ShotDir);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastImpactFX(FVector_NetQuantize ImpactPoint, FVector_NetQuantizeNormal ImpactNormal);
};
