// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EDITANYWHERE, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* Mesh1P;
	
	UPROPERTY(EDITANYWHERE, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadOnly)
	float speed;
	UPROPERTY(BlueprintReadOnly)
	float direction;
	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
