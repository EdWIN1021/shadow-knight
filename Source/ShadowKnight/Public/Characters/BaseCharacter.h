// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDCharacter.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Components/TextRenderComponent.h"
#include "BaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWKNIGHT_API ABaseCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int CurrentHP = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int AttackDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackStunDuration = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTextRenderComponent> HPText;
	
	void UpdateCurrentHP(int HP);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperZDAnimSequence> AttackAnim;
	FZDOnAnimationOverrideEndSignature AttackAnimDelegate;
};
