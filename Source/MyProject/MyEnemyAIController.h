// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"

#include "MyEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyEnemyAIController();
	

	void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly)
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditDefaultsOnly)
	class UBlackboardData* BBAsset;


	void RunAI();
};
