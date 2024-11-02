// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "BossCharacter.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS()
class SHADOWKNIGHT_API ABossCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

	virtual void ApplyDamage(float Amount) override;

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;
};
