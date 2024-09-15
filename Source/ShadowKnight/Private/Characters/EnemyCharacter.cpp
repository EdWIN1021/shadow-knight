// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "PaperZDAnimInstance.h"

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
