// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KnightPlayerState.h"
#include "GAS/KnightAbilitySystemComponent.h"
#include "GAS/KnightAttributeSet.h"

AKnightPlayerState::AKnightPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UKnightAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UKnightAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AKnightPlayerState::GetAbilitySystemComponent() const
{
	return  AbilitySystemComponent;
}
