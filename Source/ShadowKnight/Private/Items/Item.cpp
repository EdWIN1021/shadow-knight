// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Characters/KnightCharacter.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnItemBeginOverlap);
}

void AItem::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
	if(Knight && Knight->bIsAlive)
	{
		Knight->CollectItem(Type);
		Destroy();
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

