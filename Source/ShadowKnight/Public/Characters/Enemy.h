#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "KnightCharacter.h"
#include "PaperZDCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Enemy.generated.h"
/**
 * Enemy class for handling enemy behavior including player detection and movement.
 */
UCLASS()
class SHADOWKNIGHT_API AEnemy : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTextRenderComponent> HPText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int CurrentHP = 100;
	
	void ApplyDamage(int Amount, float StunDuration);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> ProximitySphere;

	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);

	UFUNCTION()
	void OnKnightLeaveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	void UpdateCurrentHP(int HP);
};
