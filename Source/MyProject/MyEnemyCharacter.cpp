// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyCharacter.h"
#include "MyEnemyAIController.h"

// Sets default values
AMyEnemyCharacter::AMyEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	//ConstructorHelpers::FClassFinder<AMyEnemyAIController> AIControllerRef(TEXT("/Game/BP/Monster/BP_MyEnemyAIController.BP_MyEnemyAIController_C"));
	//if (AIControllerRef.Class != nullptr)
	//	AIControllerClass = AIControllerRef.Class;
	//
	////AIControllerClass = AMyEnemyAIController::StaticClass();
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMyEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}
	



