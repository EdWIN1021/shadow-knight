// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowKnightGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UShadowKnightGameInstance::SetPlayerHP(float HP)
{
	PlayerHP = HP;
}

void UShadowKnightGameInstance::ChangeLevel(int level)
{
	if(level <= 0) return;
	CurrentLevel = level;

	FString LevelName = FString::Printf(TEXT("Map_%d"), level);
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelName));
}

void UShadowKnightGameInstance::RestartGame()
{
	PlayerHP = 100;
	CurrentLevel = 1;
	ChangeLevel(CurrentLevel);
}



