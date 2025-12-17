// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void AMyEnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyEnemyCharacter, bIsAttack);
	DOREPLIFETIME(AMyEnemyCharacter, health);
	DOREPLIFETIME(AMyEnemyCharacter, bOnHit);
}

void AMyEnemyCharacter::Attack(AActor* Target)
{
	if (!Target) return;
	
	if (HasAuthority())
		ServerAttack(Target);
}

void AMyEnemyCharacter::OnHit(float Damage)
{
	if (HasAuthority())
		ServerOnHit(Damage);
}

bool AMyEnemyCharacter::IsInAttackRange(const AActor* Target) const
{
	if (!Target) return false;
	
	const float distance = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
	return distance <= FMath::Square(AttackRange);
}

void AMyEnemyCharacter::ServerAttack_Implementation(AActor* Target)
{
	if (!Target) return;
	bIsAttack = IsInAttackRange(Target);
}

void AMyEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyEnemyCharacter::ServerOnHit_Implementation(float Damage)
{
	bOnHit = true;
}

