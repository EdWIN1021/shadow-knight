// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"



UENUM(BlueprintType)
enum class EItemType : uint8
{
	Key,
	HealthPotion
};

UCLASS()
class SHADOWKNIGHT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type;

	UFUNCTION()
	void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
