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
	
	UPROPERTY(Replicated)
	uint32 health = 100;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Combat")
	bool bIsAttack = false;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Combat")
	bool bOnHit = false;
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void Attack();
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void OnHit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float AttackRange = 200.f;
	
	virtual void BeginPlay() override;
	
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	
	UFUNCTION(Server, Reliable)
	void ServerOnHit();

	
	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastAttackMontage();
	
};
