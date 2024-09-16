// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "PaperZDAnimInstance.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
	ProximitySphere->SetupAttachment(RootComponent);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnKnightEnterSphere);
	ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnKnightLeaveSphere);
	UpdateCurrentHP(CurrentHP);
	AttackAnimDelegate.BindUObject(this, &AEnemyCharacter::OnAttackOverrideAnimEnd);
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnAttackCollisionBoxBeginOverlap);
}

void AEnemyCharacter::OnAttackCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
	if(Knight)
	{
		// Knight->ApplyDamage(AttackDamage, AttackStunDuration);
	}
}

void AEnemyCharacter::EnableAttackCollisionBox(bool Enabled)
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

void AEnemyCharacter::OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    AKnightCharacter* OtherKnight = Cast<AKnightCharacter>(OtherActor);
    if (OtherKnight)
    {
        Target = OtherKnight;
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
	EnableAttackCollisionBox(false);
}

void AEnemyCharacter::OnStunTimeout() 
{
	bIsStunned = false;
}


