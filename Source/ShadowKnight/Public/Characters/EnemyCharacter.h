// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KnightCharacter.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWKNIGHT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolDown = 3.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AKnightCharacter> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistanceToTarget = 30.0f;
	
	FTimerHandle AttackCoolDownTimer;

	void Attack();
	void OnAttackCoolDownTimeout();
	void OnAttackOverrideAnimEnd(bool Completed);
	
	bool ShouldFollowTarget() const;
	void MoveTowardsTarget();
	void UpdateEnemyFacingDirection(float Direction); 

	virtual void Tick(float DeltaTime) override;
	
	// ---------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStunned = false;
	
	FTimerHandle StunTimer;
	void Stun(float Duration);
	void OnStunTimeout();

};
