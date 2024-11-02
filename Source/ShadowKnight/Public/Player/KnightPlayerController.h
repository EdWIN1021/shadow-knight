// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KnightPlayerController.generated.h"

class AKnightCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SHADOWKNIGHT_API AKnightPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKnightPlayerController();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	
private:
	void Move(const FInputActionValue& Value);
	void Attack();
	void BeginJump();
	void EndJump();
	
	virtual void SetupInputComponent() override;
	void UpdateKnightFacingDirection(float Direction);
};
