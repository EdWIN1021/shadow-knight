// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.generated.h"

/**
 * 
 */

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class SHADOWKNIGHT_API ABaseCharacter : public APaperZDCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int AttackDamage = 25;


	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackStunDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsStunned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UBoxComponent> AttackCollisionBox;

	FTimerHandle StunTimer;
	void Stun(float Duration);
	void OnStunTimeout();

	virtual void ApplyDamage(int Amount);
	
	virtual void UpdateCurrentHP(float HP);

	virtual void BeginPlay() override;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UAttributeSet* GetAttributeSet(){ return AttributeSet; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InitialAttributes;

	void InitializeAttributes() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AttackAbility;
	
	void InitializeAbilities() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimSequence> AttackAnim;

	FZDOnAnimationOverrideEndSignature AttackAnimDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimSequence> DeathAnim;

	FZDOnAnimationOverrideEndSignature DeathAnimDelegate;

	void OnDeadOverrideAnimEnd(bool Completed);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UFUNCTION()
	void OnAttackCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool Enabled);
};
