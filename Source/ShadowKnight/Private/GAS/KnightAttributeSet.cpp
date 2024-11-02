// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/KnightAttributeSet.h"
#include "GameplayEffectExtension.h"

UKnightAttributeSet::UKnightAttributeSet()
{
}

void UKnightAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		float Damage = GetInComingDamage();
		SetInComingDamage(0.f);

		if(GetHealth() - Damage <= 0.f)
		{
		
		}
		else
		{
			SetHealth(GetHealth() - Damage);
		}
	}
}
