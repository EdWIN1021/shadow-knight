#include "Characters/Enemy.h"
#include "Characters/KnightCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
    ProximitySphere->SetupAttachment(RootComponent);

    HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HP"));
    HPText ->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnKnightEnterSphere);
    ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnKnightLeaveSphere);
    UpdateCurrentHP(CurrentHP);
}

void AEnemy::UpdateCurrentHP(int HP)
{
    CurrentHP = HP;
    HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), CurrentHP)));
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
        GetAnimInstance()->JumpToNode(FName("JumpToDie"));
    }
    else
    {
        Stun(StunDuration);
        GetAnimInstance()->JumpToNode(FName("JumpToHit"));
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

    AKnightCharacter* OtherKnight = Cast<AKnightCharacter>(OtherActor);
    if (OtherKnight)
    {
        Target = OtherKnight;
    }
}

void AEnemy::OnKnightLeaveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == Target)
    {
        Target = nullptr;
    }
}
