// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "PaperZDAnimInstance.h"
#include "GAS/KnightAbilitySystemComponent.h"
#include "GAS/KnightAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UKnightAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UKnightAttributeSet>(TEXT("AttributeSet"));
	
	ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
	ProximitySphere->SetupAttachment(RootComponent);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		BindDelegates();
		InitializeAttributes();
	}
	
	ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnKnightEnterSphere);
	ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnKnightLeaveSphere);
	
	AttackAnimDelegate.BindUObject(this, &AEnemyCharacter::OnAttackOverrideAnimEnd);
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnAttackCollisionBoxBeginOverlap);
}

void AEnemyCharacter::BindDelegates()
{
	if(const UKnightAttributeSet* AS = Cast<UKnightAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	}
}

void AEnemyCharacter::OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
    if (Knight)
    {
        Target = Knight;
    }
}

void AEnemyCharacter::OnKnightLeaveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == Target)
		Target = nullptr;
}

void AEnemyCharacter::Attack()
{
	 if(bIsAlive && bCanAttack && !bIsStunned){
		bCanAttack = false;
		bCanMove = false;

	 	EnableAttackCollisionBox(true);
	
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