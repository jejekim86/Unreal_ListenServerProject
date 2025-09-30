// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeaponComponent.h"

// Sets default values for this component's properties
UMyWeaponComponent::UMyWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh3P"));
}




