 // Fill out your copyright notice in the Description page of Project Settings.


#include "LevelExit.h"

#include "Characters/KnightCharacter.h"
#include "Components/CapsuleComponent.h"

 // Sets default values
ALevelExit::ALevelExit()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
}

void ALevelExit::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OnDoorBeginOverlap);
}

void ALevelExit::OnDoorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
	if(Knight && Knight->bIsAlive && bIsActive)
	{
		Knight->Deactivate();
		bIsActive = false;
		GetWorldTimerManager().SetTimer(
			WaitTimer,
			this,
			&ALevelExit::OnWaitTimeout,
			1.0f,
			false,
			1.0f);
	}
}

void ALevelExit::OnWaitTimeout()
{
	UShadowKnightGameInstance* ShadowKnightGameInstance = Cast<UShadowKnightGameInstance>(GetGameInstance());
	if(ShadowKnightGameInstance)
	{
		ShadowKnightGameInstance->ChangeLevel(Level);
	}
}
void ALevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

