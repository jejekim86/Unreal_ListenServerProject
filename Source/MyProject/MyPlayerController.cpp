// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyCharacter.h"
#include "GameFramework/Character.h"

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (MappingContext)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyPlayerController::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMyPlayerController::StopAttack);
	}
}


void AMyPlayerController::Move(const FInputActionValue& value)
{
	FVector2D Value = value.Get<FVector2D>();
	GetCharacter()->AddMovementInput(GetCharacter()->GetActorForwardVector(), Value.Y);
	GetCharacter()->AddMovementInput(GetCharacter()->GetActorRightVector(), Value.X);
}

void AMyPlayerController::Look(const FInputActionValue& value)
{
	FVector2D Value = value.Get<FVector2D>();
	GetCharacter()->AddControllerYawInput(Value.X);
	GetCharacter()->AddControllerPitchInput(-Value.Y);
}

void AMyPlayerController::Attack(const FInputActionValue& value)
{
	const bool bPressed  = value.Get<bool>();
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(GetCharacter()))
		MyCharacter->StartFire();
}

void AMyPlayerController::StopAttack(const FInputActionValue& value)
{
	const bool bPressed  = value.Get<bool>();
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(GetCharacter()))
		MyCharacter->StopFire();
}

void AMyPlayerController::Jump()
{
	GetCharacter()->Jump();
}


