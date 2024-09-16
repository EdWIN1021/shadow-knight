// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Characters/EnemyCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
	
	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HP"));
	HPText ->SetupAttachment(RootComponent);
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
		HPText->SetHiddenInGame(true);
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

void ABaseCharacter::UpdateCurrentHP(int HP)
{
	CurrentHP = HP;
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), CurrentHP)));
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
