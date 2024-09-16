#include "Characters/Enemy.h"
#include "Characters/KnightCharacter.h"
#include "Components/SphereComponent.h"

AEnemy::AEnemy()
{
    ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
    ProximitySphere->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnKnightEnterSphere);
    ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnKnightLeaveSphere);
}

void AEnemy::OnKnightEnterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

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
        Target = nullptr;
}
