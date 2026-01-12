// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyItemData.h"
#include "MyBullet.h"
#include "Curves/CurveVector.h"
#include "MyWeaponData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle = 0,
	Pistol,
	Shotgun,
	None
};
USTRUCT(BlueprintType)
struct FWeaponIKData
{
	GENERATED_BODY()

	// 무기 메시 소켓 이름(무기 BP의 소켓)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LeftHandIKSocket = TEXT("lh_ik");  // 무기 메시에 있는 왼손 타깃 소켓

	// 3인칭용: 앵커/소켓에 붙인 뒤 추가 보정(상대 변환)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform GripTPOffset = FTransform::Identity;

	// 1인칭용: 팔 메시 기준 보정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform GripFPOffset = FTransform::Identity;

	// IK 세기/블렌드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="0.0", ClampMax="1.0"))
	float LeftHandIKAlpha = 1.0f;

	// ADS 시 보정(선택)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform ADS_FP_Offset = FTransform::Identity;

	// (선택) 반동/스웨이 커브
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveVector* RecoilCurve;
};
UCLASS()
class MYPROJECT_API UMyWeaponData : public UMyItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint16 Damage;

	UPROPERTY(EditAnywhere)
	float FireRate;

	UPROPERTY(EditAnywhere)
	float ReloadTime;

	UPROPERTY(EditAnywhere)
	float Spread;

	UPROPERTY(EditAnywhere)
	bool bAuto;

	UPROPERTY(EditAnywhere)
	uint16 BulletCount;

	UPROPERTY(EditAnywhere)
	uint16 SwitchingBulletCount;

	UPROPERTY(EditAnywhere)
	UParticleSystem* FireTracerFX;   // 투사체 Trail로 사용 (날아가는 느낌)

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactFX;       // 추가(없으면 nullptr 가능)

	UPROPERTY(EditAnywhere)
	float HeadshotMultiplier = 2.0f; // 추가

	UPROPERTY(EditAnywhere)
	FName HeadBoneName = TEXT("head"); // 추가 (스켈레톤에 맞게)

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> BodyDamageType; // 수정/추가

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> HeadDamageType; // 추가

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyBullet> BulletClass; // 추가(없으면 기본 AMyBullet)

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	FWeaponIKData IK;

	UPROPERTY(EditAnywhere)
	UAnimSequence* FireAnim;

	UPROPERTY(EditAnywhere)
	UAnimSequence* ReloadAnim;
};

