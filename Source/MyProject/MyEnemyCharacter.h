// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyEnemyCharacter();


	virtual void BeginPlay() override;
};
