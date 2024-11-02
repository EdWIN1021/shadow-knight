// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "KnightGameplayTag.h"
#include "Engine/DamageEvents.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/KnightAttributeSet.h"

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
		UE_LOG(LogTemp, Warning, TEXT("here"));
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

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::InitializeAttributes() const
{
	if(GetAbilitySystemComponent() && InitialAttributes)
	{
		const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		const FGameplayEffectSpecHandle  SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitialAttributes, 1.0f, ContextHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ABaseCharacter::InitializeAbilities() const
{
	FGameplayAbilitySpec AbilitySpec(AttackAbility);
	AbilitySpec.SourceObject = AbilitySystemComponent->GetAvatarActor();
	AbilitySpec.DynamicAbilityTags.AddTag(KnightGameplayTags::Player_Ability_Attack);
	AbilitySystemComponent->GiveAbility(AbilitySpec);
}

void ABaseCharacter::OnAttackCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(OtherActor);
	if (HitCharacter && HitCharacter != this)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();
		UKnightAttributeSet* SourceAS =  Cast<UKnightAttributeSet>(GetAttributeSet());
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, SourceASC->MakeEffectContext());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, KnightGameplayTags::Damage, SourceAS->GetStrength());

		if(UAbilitySystemComponent* TargetASC = HitCharacter->GetAbilitySystemComponent())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		};
		
		// HitCharacter->ApplyDamage(AttackDamage, AttackStunDuration);
		// FDamageEvent TargetAttackedEvent;
		// HitCharacter->TakeDamage(
		// 	AttackDamage,
		// 	TargetAttackedEvent,
		// 	GetOwner()->GetInstigatorController(),
		// 	GetOwner());
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
