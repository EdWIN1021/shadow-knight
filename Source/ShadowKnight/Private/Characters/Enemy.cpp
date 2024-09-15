#include "Characters/Enemy.h"
#include "Characters/KnightCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and initialize the proximity sphere component
    ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
    ProximitySphere->SetupAttachment(RootComponent);

    HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HP"));
    HPText ->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Bind overlap events to the proximity sphere
    ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnKnightEnterSphere);
    ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnKnightLeaveSphere);
    UpdateCurrentHP(CurrentHP);
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ShouldFollowTarget())
    {
        MoveTowardsTarget();
    }
    else
    {
        // Perform attack or other action when not following the target
    }
}

bool AEnemy::ShouldFollowTarget() const
{
    if (!Target || !bIsAlive || !bCanMove)
    {
        return false;
    }

    // Calculate the distance to the target
    float DistanceToTarget = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

    // Check if the enemy should stop based on the stop distance
    return DistanceToTarget > StopDistanceToTarget;
}

void AEnemy::UpdateCurrentHP(int HP)
{
    CurrentHP = HP;
    HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), CurrentHP)));
}


void AEnemy::MoveTowardsTarget()
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

void AEnemy::UpdateEnemyFacingDirection(float Direction)
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

void AEnemy::ApplyDamage(int Amount, float StunDuration)
{
    if(!bIsAlive) return;
    UpdateCurrentHP(CurrentHP - Amount);

    if(CurrentHP <= 0)
    {
        UpdateCurrentHP(0);
        HPText->SetHiddenInGame(true);
        bIsAlive = false;
        bCanMove = false;
    }
    else
    {
        
    }
}

void AEnemy::OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    // Check if the overlapped actor is a KnightCharacter
    AKnightCharacter* OtherKnight = Cast<AKnightCharacter>(OtherActor);
    if (OtherKnight)
    {
        // Set the target to the KnightCharacter
        Target = OtherKnight;
    }
}

void AEnemy::OnKnightLeaveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // If the actor leaving is the current target, clear the target
    if (OtherActor == Target)
    {
        Target = nullptr;
    }
}
