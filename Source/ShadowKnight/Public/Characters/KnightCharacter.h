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

	/** SpringArm for positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Side-scrolling camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	TObjectPtr<UBoxComponent> AttackCollisionBox;
	
	/** Input mapping context for enhanced input system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	/** Input action for movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Input action for attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	/** Input action for jumping */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackStunDuration = 0.3f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Update the knight's facing direction based on movement input */
	void UpdateKnightFacingDirection(float Direction);

	/** Handles the knight's movement input */
	void Move(const FInputActionValue& Value);

	/** Handles the knight's attack input */
	void Attack(const FInputActionValue& Value);

	/** Handles the knight's jump start */
	void BeginJump(const FInputActionValue& Value);

	/** Handles the knight's jump end */
	void EndJump(const FInputActionValue& Value);



	

	/** Called when an attack animation completes */ 
	void OnAttackAnimationComplete(bool Completed);

	UFUNCTION()
	void OnAttackCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool Enabled);
};
