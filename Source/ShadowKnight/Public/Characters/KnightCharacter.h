#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "ShadowKnightGameInstance.h"
#include "Items/Item.h"
#include "KnightCharacter.generated.h"

/**
 * AKnightCharacter represents the player character with movement, attack, and animation capabilities.
 */
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UShadowKnightGameInstance* ShadowKnightGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* PickUpSound;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	void CollectItem(EItemType Type);
	virtual void UpdateCurrentHP(float HP) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void UpdateKnightFacingDirection(float Direction);

	void Move(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void BeginJump(const FInputActionValue& Value);

	void EndJump(const FInputActionValue& Value);

	void OnAttackAnimationComplete(bool Completed);
};
