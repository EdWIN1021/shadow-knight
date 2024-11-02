// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/KnightAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Characters/BaseCharacter.h"

class ABaseCharacter;

UKnightAttributeSet::UKnightAttributeSet()
{
}

void UKnightAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid() )
		{
			float Damage = GetInComingDamage();
			SetInComingDamage(0.f);
			ABaseCharacter* TargetAvatarActor = Cast<ABaseCharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());
			TargetAvatarActor->ApplyDamage(GetHealth() - Damage);
			SetHealth(GetHealth() - Damage);
		}
	}
}
