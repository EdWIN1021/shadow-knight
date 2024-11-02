// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"


void ABossCharacter::ApplyDamage(float Amount)
{
	Super::ApplyDamage(Amount);

	if(Amount <= 0 )
	{
		OnDeath.Broadcast();
	}
}
