#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "KnightCharacter.generated.h"

/**
 * AKnightCharacter represents the player character with movement, attack, and animation capabilities.
 */
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	TObjectPtr<UBoxComponent> AttackCollisionBox;
	
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

	UFUNCTION()
	void OnAttackCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool Enabled);
};
