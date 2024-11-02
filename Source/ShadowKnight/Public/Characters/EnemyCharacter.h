// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KnightCharacter.h"
#include "Characters/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "UI/KnightHUD.h"
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
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AKnightCharacter> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistanceToTarget = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolDown = 3.0f;

	FTimerHandle AttackCoolDownTimer;

	void Attack();
	
	void OnAttackCoolDownTimeout();
	
	void OnAttackOverrideAnimEnd(bool Completed);
	
	bool ShouldFollowTarget() const;
	
	void MoveTowardsTarget();
	
	void UpdateEnemyFacingDirection(float Direction); 

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category="Enemy Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Enemy Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> ProximitySphere;

	void BindDelegates();

private:
	UFUNCTION()
	void OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);

	UFUNCTION()
	void OnKnightLeaveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);
};
