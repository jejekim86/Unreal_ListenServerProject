// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyItemData.h"
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
	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	float ShootSpeed;
	UPROPERTY(EditAnywhere)
	float ReloadTime;
	UPROPERTY(EditAnywhere)
	float Spread; // 흔들림
	UPROPERTY(EditAnywhere)
	uint8 Firemode;
	UPROPERTY(EditAnywhere)
	uint16 CurrentBulletcount;
	UPROPERTY(EditAnywhere)
	uint16 MaxBulletCount;
	

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere)
	FWeaponIKData IK;

};
