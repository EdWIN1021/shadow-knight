#include "Characters/KnightCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"


AKnightCharacter::AKnightCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
 
	// Attach the ViewCamera to SpringArm
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	AttackAnimDelegate.BindUObject(this, &AKnightCharacter::OnAttackAnimationComplete);
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
			break; 
		
	    case EItemType::HealthPotion:
			break;

		default:
			break;
	}
}

void AKnightCharacter::UpdateCurrentHP(float HP)
{
	Super::UpdateCurrentHP(HP);
	if (ShadowKnightGameInstance)
	{
		ShadowKnightGameInstance->SetPlayerHP(CurrentHP);
	}
}



void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKnightCharacter::Move);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKnightCharacter::Attack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKnightCharacter::BeginJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKnightCharacter::EndJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &AKnightCharacter::EndJump);
	}
}

void AKnightCharacter::UpdateKnightFacingDirection(float Direction)
{
	if (Controller)
	{
		FRotator CurrentRotation = Controller->GetControlRotation();
		if (Direction < 0.0f && CurrentRotation.Yaw != 180.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
		}
		else if (Direction > 0.0f && CurrentRotation.Yaw != 0.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
		}
	}
}

void AKnightCharacter::Move(const FInputActionValue& Value)
{
	if (bCanMove && bIsAlive && !bIsStunned)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value.Get<float>());
		UpdateKnightFacingDirection(Value.Get<float>());
	}
}

void AKnightCharacter::Attack(const FInputActionValue& Value)
{
	if (bIsAlive && bCanAttack && !bIsStunned)
	{
		bCanAttack = false;
		bCanMove = false;

		EnableAttackCollisionBox(true);

		if (UPaperZDAnimInstance* AnimInstance = GetAnimInstance())
		{
			AnimInstance->PlayAnimationOverride(AttackAnim, FName("DefaultSlot"), 1.0f, 0.0f, AttackAnimDelegate);
		}
	}
}

void AKnightCharacter::OnAttackAnimationComplete(bool Completed)
{
	bCanAttack = true;
	bCanMove = true;
	EnableAttackCollisionBox(false);
}

void AKnightCharacter::BeginJump(const FInputActionValue& Value)
{
	if (bCanMove && bIsAlive)
		Jump();
}

void AKnightCharacter::EndJump(const FInputActionValue& Value)
{
	StopJumping();
}
