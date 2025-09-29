// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMyWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyWeaponComponent();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh3P;
};
