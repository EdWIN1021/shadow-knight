// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShadowKnightGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWKNIGHT_API UShadowKnightGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PlayerHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentLevel = 1;
		
	void SetPlayerHP(float HP);
	void ChangeLevel(int level);

	UFUNCTION(BlueprintCallable)
	void RestartGame();
};
