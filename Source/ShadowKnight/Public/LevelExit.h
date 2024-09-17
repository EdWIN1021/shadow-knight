// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LevelExit.generated.h"

UCLASS()
class SHADOWKNIGHT_API ALevelExit : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelExit();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsActive = true;

	FTimerHandle WaitTimer;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnDoorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);

	void OnWaitTimeout();
	virtual void Tick(float DeltaTime) override;
};
