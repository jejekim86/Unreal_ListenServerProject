// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AMyEnemyAIController::AMyEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/BP/Monster/BT_Enemy.BT_Enemy"));
	if (BehaviorTreeRef.Object != nullptr)
		BTAsset = BehaviorTreeRef.Object;

	ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardRef(TEXT("/Game/BP/Monster/BB_Enemy.BB_Enemy"));
	if (BlackboardRef.Object != nullptr)
		BBAsset = BlackboardRef.Object;
}

void AMyEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BTAsset != nullptr)
		RunBehaviorTree(BTAsset);
	
		RunAI();
}

void AMyEnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector("HomePos",GetPawn()->GetActorLocation());
		RunBehaviorTree(BTAsset);
	}
}
