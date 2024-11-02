// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "KnightAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class SHADOWKNIGHT_API UKnightAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UKnightAttributeSet();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UKnightAttributeSet, Health);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UKnightAttributeSet, MaxHealth);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UKnightAttributeSet, Strength);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Attributes")
	FGameplayAttributeData InComingDamage;
	ATTRIBUTE_ACCESSORS(UKnightAttributeSet, InComingDamage);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};

