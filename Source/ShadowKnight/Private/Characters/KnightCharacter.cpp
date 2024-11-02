#include "Characters/KnightCharacter.h"

#include "AbilitySystemComponent.h"
#include "KnightGameplayTag.h"
#include "Characters/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/KnightPlayerController.h"
#include "Player/KnightPlayerState.h"
#include "UI/KnightHUD.h"


AKnightCharacter::AKnightCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
 
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
	bHasKey = false;

	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AKnightCharacter::AKnightCharacter::OnAttackCollisionBoxBeginOverlap);
	EnableAttackCollisionBox(false);

	ShadowKnightGameInstance = Cast<UShadowKnightGameInstance>(GetGameInstance());
	if(ShadowKnightGameInstance)
	{
		CurrentHP = ShadowKnightGameInstance->PlayerHP;
	}
}

void AKnightCharacter::CollectItem(EItemType Type)
{
	UGameplayStatics::PlaySound2D(GetWorld(), PickUpSound);

	switch (Type)
	{
		case EItemType::Key:
			bHasKey = true;
			break; 
		
	    case EItemType::HealthPotion:
	    	UpdateCurrentHP(100);
			break;
	
		default:
			break;
	}
}

void AKnightCharacter::OnRestartTimeout()
{
	 ShadowKnightGameInstance->RestartGame();
}

void AKnightCharacter::UpdateCurrentHP(float HP)
{
	Super::UpdateCurrentHP(HP);

	OnHealthChanged.Broadcast();
	
	if (ShadowKnightGameInstance)
	{
		ShadowKnightGameInstance->SetPlayerHP(CurrentHP);
	}
}

void AKnightCharacter::ApplyDamage(int Amount)
{
	if(!bIsActive) return;

	Super::ApplyDamage(Amount);
	
	if(CurrentHP <= 0)
	{
		GetWorldTimerManager().SetTimer(
			RestartTimer,
			this,
			&AKnightCharacter::OnRestartTimeout,
			1.0f,
			false,
			3.0f
			);
	}
}

void AKnightCharacter::Deactivate()
{
	if(bIsActive)
	{
		bIsActive = false;
		bCanAttack = false;
		bCanMove = false;
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void AKnightCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AKnightPlayerState* PS = Cast<AKnightPlayerState>(GetPlayerState()))
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AttributeSet = PS->GetAttributeSet();

		AKnightPlayerController* PC =  Cast<AKnightPlayerController>(GetController());
		AKnightHUD* KnightHUD = Cast<AKnightHUD>(PC->GetHUD());
		
		KnightHUD->InitializedHUD();
		KnightHUD->BindDelegates(AbilitySystemComponent, AttributeSet);

		InitializeAttributes();
		
		InitializeAbilities();
	}
}

void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKnightCharacter::Attack()
{
	if (bIsAlive && bCanAttack && !bIsStunned)
	{
		bCanAttack = false;
		bCanMove = false;

		EnableAttackCollisionBox(true);

		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		for(const FGameplayAbilitySpec& AbilitySpec: ASC->GetActivatableAbilities())
		{
			if(AbilitySpec.DynamicAbilityTags.HasTagExact(KnightGameplayTags::Player_Ability_Attack))
			{
				ASC->TryActivateAbility(AbilitySpec.Handle);
				return;
			}
		}
	}
}

void AKnightCharacter::OnAttackAnimationComplete()
{
	if(bIsAlive && bIsActive)
	{
		bCanAttack = true;
		bCanMove = true;
	}
	EnableAttackCollisionBox(false);
}
