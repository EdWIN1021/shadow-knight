// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "PaperZDAnimInstance.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HP"));
	HPText ->SetupAttachment(RootComponent);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateCurrentHP(CurrentHP);
	AttackAnimDelegate.BindUObject(this, &AEnemyCharacter::OnAttackOverrideAnimEnd);
}

void AEnemyCharacter::UpdateCurrentHP(int HP)
{
	CurrentHP = HP;
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), CurrentHP)));
}

void AEnemyCharacter::Attack()
{
	 if(bIsAlive && bCanAttack && !bIsStunned){
		bCanAttack = false;
		bCanMove = false;
	
	 	GetAnimInstance()->PlayAnimationOverride(AttackAnim, FName("DefaultSlot"), 1.0f, 0.0f, AttackAnimDelegate);
	 	GetWorldTimerManager().SetTimer(
			 AttackCoolDownTimer,
			 this,
			 &AEnemyCharacter::OnAttackCoolDownTimeout,
			 1.0f,
			 false,
			 AttackCoolDown);
	 }
}

void AEnemyCharacter::OnAttackCoolDownTimeout()
{
	if(bIsAlive) bCanAttack = true;
}

void AEnemyCharacter::OnAttackOverrideAnimEnd(bool Completed)
{
	if(bIsAlive) bCanMove = true;
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
		float FacingDirection = (Target->GetActorLocation().X - GetActorLocation().X) > 0.0f ? 1.0f : -1.0f;
		UpdateEnemyFacingDirection(FacingDirection);
        
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

void AEnemyCharacter::ApplyDamage(int Amount, float StunDuration)
{
	if(!bIsAlive) return;
	UpdateCurrentHP(CurrentHP - Amount);

	if(CurrentHP <= 0)
	{
		UpdateCurrentHP(0);
		HPText->SetHiddenInGame(true);
		bIsAlive = false;
		bCanMove = false;
		bCanAttack = false;
		GetAnimInstance()->JumpToNode(FName("JumpToDie"));
	}
	else
	{
		Stun(StunDuration);
		GetAnimInstance()->JumpToNode(FName("JumpToHit"));
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
		if(Target && Target->bIsAlive)
		{
			Attack();
		}
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


