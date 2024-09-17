// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "Engine/DamageEvents.h"

ABaseCharacter::ABaseCharacter()
{
	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);

}

void ABaseCharacter::Stun(float Duration)
{
	bIsStunned = true;
	bool bIsTimerActive = GetWorldTimerManager().IsTimerActive(StunTimer);
	if(bIsTimerActive)
	{
		GetWorldTimerManager().ClearTimer(StunTimer);
	}
	
	GetWorldTimerManager().SetTimer(StunTimer, this, &ABaseCharacter::OnStunTimeout, 1.0f, false, Duration);
	GetAnimInstance()->StopAllAnimationOverrides();
	EnableAttackCollisionBox(false);
}

void ABaseCharacter::OnStunTimeout()
{
	bIsStunned = false;
}

void ABaseCharacter::ApplyDamage(int Amount, float StunDuration)
{
	if(!bIsAlive) return;
	Stun(StunDuration);
	UpdateCurrentHP(CurrentHP - Amount);
  
	if(CurrentHP <= 0)
	{
		UpdateCurrentHP(0);
		bIsAlive = false;
		bCanMove = false;
		bCanAttack = false;
		GetAnimInstance()->JumpToNode(FName("JumpToDie"));
		EnableAttackCollisionBox(false);
	}
	else
	{
		GetAnimInstance()->JumpToNode(FName("JumpToHit"));
	}
}

void ABaseCharacter::UpdateCurrentHP(float HP)
{
	CurrentHP = HP;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateCurrentHP(CurrentHP);
}

void ABaseCharacter::OnAttackCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(OtherActor);
	if (HitCharacter && HitCharacter != this)
	{
		HitCharacter->ApplyDamage(AttackDamage, AttackStunDuration);
		FDamageEvent TargetAttackedEvent;
		HitCharacter->TakeDamage(
			AttackDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner());
	}
}

void ABaseCharacter::EnableAttackCollisionBox(bool Enabled)
{
	if(Enabled)
	{
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else {
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}
