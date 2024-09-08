#pragma once

#include "CoreMinimal.h"
#include "KnightCharacter.h"
#include "PaperZDCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Enemy.generated.h"
/**
 * Enemy class for handling enemy behavior including player detection and movement.
 */
UCLASS()
class SHADOWKNIGHT_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AKnightCharacter> Target;

	/** Distance at which the enemy should stop moving towards the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistanceToTarget = 30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanMove = true; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTextRenderComponent> HPText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int CurrentHP = 100;
	

protected:
	/** Proximity sphere used for detecting players. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> ProximitySphere;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void MoveTowardsTarget();
	void UpdateEnemyFacingDirection(float Direction); 

private:
	/** Handles when a player enters the proximity sphere. */
	UFUNCTION()
	void OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);

	UFUNCTION()
	void OnKnightLeaveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	bool ShouldFollowTarget() const;
	void UpdateCurrentHP(int HP);
};
