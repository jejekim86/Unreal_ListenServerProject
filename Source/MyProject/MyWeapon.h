// MyWeapon.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class USphereComponent;
class UAnimationAsset;
class UMyWeaponData;
class AMyBullet;

UCLASS()
class MYPROJECT_API AMyWeapon : public AActor
{
	GENERATED_BODY()

public:
	AMyWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USphereComponent* SphereColl = nullptr;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UMyWeaponData* WeaponData = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName MuzzleName = TEXT("MuzzleFlash");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float Range = 15000.f;	

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Weapon|Ammo")
	int32 BulletCount = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Weapon|Ammo")
	int32 ReserveAmmo = 0;

	FTimerHandle AutoFireTimer;

public:
	// 캐릭터(Server RPC)에서 호출되는 서버 전용 함수
	void StartFire_Server();
	void StopFire_Server();
	void HandleFire_Server();
	void Reload_Server();

protected:
	// 무기 애니만 (원하면 이것도 제거하고 Owner 로컬에서만 재생해도 됨)
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayWeaponAnim(UAnimationAsset* Anim);

	bool BuildAimPoint_Server(FVector& OutAimPoint) const;

	UFUNCTION()
	void OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								   const FHitResult& SweepResult);
};
