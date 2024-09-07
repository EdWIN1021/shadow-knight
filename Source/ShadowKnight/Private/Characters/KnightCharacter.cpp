// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KnightCharacter.h"

AKnightCharacter::AKnightCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// create a SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// attach the ViewCamera to SpringArm
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKnightCharacter::Move);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKnightCharacter::Attack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKnightCharacter::BeginJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKnightCharacter::EndJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &AKnightCharacter::EndJump);
	}
}

void AKnightCharacter::UpdateKnightFacingDirection(float Direction)
{
	if (Controller)
	{
		FRotator CurrentRotation = Controller->GetControlRotation();
		if (Direction < 0.0f && CurrentRotation.Yaw != 180.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
		}
		else if (Direction > 0.0f && CurrentRotation.Yaw != 0.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
		}
	}

}

void AKnightCharacter::Move(const FInputActionValue& Value)
{
	if(bCanMove && bIsActive)
	{
		AddMovementInput(FVector(1.0f,0.0f,0.0f), Value.Get<float>());
		UpdateKnightFacingDirection(Value.Get<float>());
	}
}

void AKnightCharacter::Attack(const FInputActionValue& Value)
{
	
}

void AKnightCharacter::BeginJump(const FInputActionValue& Value)
{
	if(bCanMove && bIsActive)
		Jump();
}

void AKnightCharacter::EndJump(const FInputActionValue& Value)
{
	StopJumping();
}
