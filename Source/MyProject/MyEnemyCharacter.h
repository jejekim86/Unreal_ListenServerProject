// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "MyEnemyCharacter.generated.h"


UCLASS()
class MYPROJECT_API AMyEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyEnemyCharacter();
	
	UPROPERTY(Replicated)
	uint32 health = 100;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsAttack = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Combat")
	bool bOnHit = false;
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void Attack(AActor* Target);
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void OnHit(float Damage);
	
	UFUNCTION(BlueprintCallable)
	bool IsInAttackRange(AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float AttackRange = 200.f;
	
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UFUNCTION(Server, Reliable)
	void ServerAttack(AActor* Target);
	
	UFUNCTION(Server, Reliable)
	void ServerOnHit(float Damage);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastAttackMontage();
};
