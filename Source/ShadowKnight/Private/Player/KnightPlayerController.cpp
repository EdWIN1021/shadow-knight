// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KnightPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/KnightCharacter.h"
#include "GameFramework/Character.h"


AKnightPlayerController::AKnightPlayerController()
{
}

void AKnightPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKnightPlayerController::Move);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKnightPlayerController::Attack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKnightPlayerController::BeginJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKnightPlayerController::EndJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &AKnightPlayerController::EndJump);
	}
}

void AKnightPlayerController::UpdateKnightFacingDirection(float Direction)
{
	FRotator CurrentRotation = GetControlRotation();
	if (Direction < 0.0f && CurrentRotation.Yaw != 180.0f)
	{
		SetControlRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
	}
	else if (Direction > 0.0f && CurrentRotation.Yaw != 0.0f)
	{
		SetControlRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
	}
}

void AKnightPlayerController::Move(const FInputActionValue& Value)
{
	AKnightCharacter* Knight = Cast<AKnightCharacter>(GetCharacter());
	if (Knight->bCanMove && Knight->bIsAlive && !Knight->bIsStunned)
	{
		Knight->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value.Get<float>());
		UpdateKnightFacingDirection(Value.Get<float>());
	}
}

void AKnightPlayerController::Attack()
{
	AKnightCharacter* Knight = Cast<AKnightCharacter>(GetCharacter());
	Knight->Attack();
}

void AKnightPlayerController::BeginJump()
{
	AKnightCharacter* Knight = Cast<AKnightCharacter>(GetCharacter());
	if (Knight->bCanMove && Knight->bIsAlive)
	GetCharacter()->Jump();
}

void AKnightPlayerController::EndJump()
{
	GetCharacter()->StopJumping();
}
