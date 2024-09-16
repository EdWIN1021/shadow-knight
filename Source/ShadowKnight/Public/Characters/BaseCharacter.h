// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDCharacter.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "BaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWKNIGHT_API ABaseCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	bool bCanMove = true; 

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperZDAnimSequence> AttackAnim;
	FZDOnAnimationOverrideEndSignature AttackAnimDelegate;
};
