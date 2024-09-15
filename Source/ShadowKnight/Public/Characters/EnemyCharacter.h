// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWKNIGHT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStunned = false;

	FTimerHandle StunTimer;

	void Stun(float Duration);
	void OnStunTimeout();
};
