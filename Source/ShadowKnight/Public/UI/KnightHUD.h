// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KnightHUD.generated.h"

/**
 * 
 */

class UAttributeSet;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, newValue);

UCLASS()
class SHADOWKNIGHT_API AKnightHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializedHUD();

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	void BindDelegates(UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet) const;
};
