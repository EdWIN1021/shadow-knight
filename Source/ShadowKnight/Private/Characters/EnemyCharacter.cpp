// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "PaperZDAnimInstance.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::Attack()
{
	 
}

void AEnemyCharacter::OnAttackCoolDownTimeout()
{
	
}

void AEnemyCharacter::OnAttackOverrideAnimEnd(bool Completed)
{
	
}

bool AEnemyCharacter::ShouldFollowTarget() const
{
	if (!Target || !bIsAlive || !bCanMove)
	{
		return false;
	}

	float DistanceToTarget = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

	return DistanceToTarget > StopDistanceToTarget;
}

void AEnemyCharacter::MoveTowardsTarget()
{
	if (Target)
	{
		// Determine the direction (1 for right, -1 for left)
		float FacingDirection = (Target->GetActorLocation().X - GetActorLocation().X) > 0.0f ? 1.0f : -1.0f;
		UpdateEnemyFacingDirection(FacingDirection);
        
		// Move in the direction of the target
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), FacingDirection);
	}
}

void AEnemyCharacter::UpdateEnemyFacingDirection(float Direction)
{
	FRotator CurrentRotation = GetActorRotation(); 

	if (Direction < 0.0f && CurrentRotation.Yaw != 180.0f)
	{
		SetActorRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
	}
	else if (Direction > 0.0f && CurrentRotation.Yaw != 0.0f)
	{
		SetActorRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAlive && ShouldFollowTarget() && !bIsStunned)
	{
		MoveTowardsTarget();
	}
	else
	{
		// Perform attack or other action when not following the target
	}
}

void AEnemyCharacter::Stun(float Duration)
{
	bIsStunned = true;

	bool bIsTimerActive = GetWorldTimerManager().IsTimerActive(StunTimer);
	if(bIsTimerActive)
	{
		GetWorldTimerManager().ClearTimer(StunTimer);
	}
	
	GetWorldTimerManager().SetTimer(StunTimer, this, &AEnemyCharacter::OnStunTimeout, 1.0f, false, Duration);
	GetAnimInstance()->StopAllAnimationOverrides();
}

void AEnemyCharacter::OnStunTimeout() 
{
	bIsStunned = false;
}
