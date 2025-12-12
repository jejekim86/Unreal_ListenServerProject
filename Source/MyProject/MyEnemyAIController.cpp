// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Perception/AIPerceptionComponent.h"

AMyEnemyAIController::AMyEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/BP/Monster/BT_Enemy.BT_Enemy"));
	if (BehaviorTreeRef.Object != nullptr)
		BTAsset = BehaviorTreeRef.Object;
		
	ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardRef(TEXT("/Game/BP/Monster/BB_Enemy.BB_Enemy"));
	if (BlackboardRef.Object != nullptr)
		BBAsset = BlackboardRef.Object;
	
	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 550.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies   = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals  = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMyEnemyAIController::OnTargetPerceptionUpdated);
}

void AMyEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	//if (BTAsset != nullptr)
		//RunBehaviorTree(BTAsset);
	
		RunAI();
}

void AMyEnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		//Blackboard->SetValueAsObject("AttackTarget", GetWorld()->GetFirstPlayerController()->GetPawn());
		RunBehaviorTree(BTAsset);
	}
}


void AMyEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{	
	if (GetLocalRole() != ROLE_Authority) return;

	UBlackboardComponent* BBComp = GetBlackboardComponent();
	
	if (!BBComp) return;
	
	switch (Stimulus.WasSuccessfullySensed())
	{
	case true:
		if (APawn* pawn = Cast<APawn>(Actor))
		{
			if (pawn->IsPlayerControlled())
				Blackboard->SetValueAsObject("AttackTarget", pawn);
		}
		break;
	case false:
		if (Actor == Blackboard->GetValueAsObject("AttackTarget"))
			Blackboard->ClearValue("AttackTarget");
		break;
	}
}
