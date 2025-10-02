// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyItemData.generated.h"

/**
 * 
 */

UCLASS()
class MYPROJECT_API UMyItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	USkeletalMesh* Mesh;
};
