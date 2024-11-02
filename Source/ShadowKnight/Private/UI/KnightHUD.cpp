// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KnightHUD.h"

#include "GAS/KnightAttributeSet.h"

void AKnightHUD::BindDelegates(UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet) const
{
	const UKnightAttributeSet* AS = Cast<UKnightAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AS->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
}
