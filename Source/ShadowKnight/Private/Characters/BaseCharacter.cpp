// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HP"));
	HPText ->SetupAttachment(RootComponent);
}

void ABaseCharacter::UpdateCurrentHP(int HP)
{
	CurrentHP = HP;
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), CurrentHP)));
}
