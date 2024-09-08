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
    
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Bind overlap events to proximity sphere
    ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnKnightEnterSphere);
    ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnKnightLeaveSphere);
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(ShouldFollowTarget())
    {
        if (bIsAlive && Target && bCanMove)
        {
            // Calculate facing direction (1 if target is to the right, -1 if left)
            float FacingDirection = (Target->GetActorLocation().X - GetActorLocation().X) > 0.0f ? 1.0f : -1.0f;

            // Add movement input towards the target
            AddMovementInput(FVector(1.0f, 0.0f, 0.0f), FacingDirection);
        }
    } else
    {
        //Attack
    }
}

bool AEnemy::ShouldFollowTarget()
{
    bool Result = false;
    if(Target)
    {
        float DistanceToTarget = abs(Target->GetActorLocation().X - GetActorLocation().X);
        Result = DistanceToTarget > StopDistanceToTarget;
    }
    return  Result;
}

void AEnemy::OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
    if (OtherActor == Target)
    {
        // Clear the target when the player leaves the sphere
        Target = nullptr;
    }
}


