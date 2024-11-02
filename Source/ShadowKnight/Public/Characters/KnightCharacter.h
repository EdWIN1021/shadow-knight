#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ShadowKnightGameInstance.h"
#include "Items/Item.h"
#include "KnightCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);

UCLASS()
class SHADOWKNIGHT_API AKnightCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AKnightCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UShadowKnightGameInstance* ShadowKnightGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* PickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasKey = false;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	FTimerHandle RestartTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = true;
	
	void CollectItem(EItemType Type);

	void OnRestartTimeout();
	
	virtual void ApplyDamage(float Amount) override;

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	virtual void PossessedBy(AController* NewController) override;

	void Attack();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnAttackAnimationComplete();
};
