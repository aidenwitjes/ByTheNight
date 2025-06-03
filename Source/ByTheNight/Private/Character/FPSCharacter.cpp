// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/FPSCharacter.h"
#include "InteractionManager.h"
#include "FPSProjectGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterObjects/Lantern.h"
#include "Kismet/GameplayStatics.h"
#include "UGameHUDWidget.h"
#include "UPauseMenuWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!FPSCameraComponent) {
		FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
		FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
		FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
		FPSCameraComponent->bUsePawnControlRotation = true;
	}

	LanternAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("LanternAnchor"));
	LanternAnchor->SetupAttachment(FPSCameraComponent);
	LanternAnchor->SetRelativeLocation(LanternAnchorOffset);
	LanternAnchor->SetRelativeRotation(LanternAnchorRotation);

	// Create the Interaction Manager component
	InteractionManager = CreateDefaultSubobject<UInteractionManager>(TEXT("InteractionManager"));
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set initial walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Ensure LanternAnchor is valid and apply transform based on configurable offset/rotation
	if (LanternAnchor)
	{
		LanternAnchor->SetRelativeLocation(LanternAnchorOffset);
		LanternAnchor->SetRelativeRotation(LanternAnchorRotation);
	}

	// Spawn the Lantern if LanternClass is set
	if (LanternClass && LanternAnchor)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		Lantern = GetWorld()->SpawnActor<ALantern>(LanternClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (Lantern)
		{
			Lantern->AttachToComponent(LanternAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Don't update gameplay logic if paused
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		return;
	}

	// Display data for movement speed (testing sprinting logic)
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Speed: %f"), GetCharacterMovement()->Velocity.Size()));

	// Check for interactables in crosshair and display UI feedback
	if (InteractionManager)
	{
		InteractionManager->CheckForInteractables(this);
	}

	// Update HUD widget with current values
	if (GetWorld())
	{
		AFPSProjectGameMode* GM = Cast<AFPSProjectGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			UGameHUDWidget* HUD = GM->GetHUDWidget();
			if (HUD)
			{
				HUD->SetSheepCount(CurrentSheepCount, MaxSheepCount);
				HUD->SetStamina(StaminaPercent);
				HUD->SetFuel(Lantern ? Lantern->GetFuelPercent() : 0.0f);
			}
		}
	}

	// Stamina logic
	if (bIsSprinting)
	{
		// Drain stamina
		StaminaPercent -= StaminaDrainRate * DeltaTime;
		StaminaPercent = FMath::Clamp(StaminaPercent, 0.0f, 1.0f);

		// Stop sprinting if out of stamina
		if (StaminaPercent <= 0.0f)
		{
			StopSprinting();
		}

		// Reset recharge timer since we're sprinting
		TimeSinceSprintEnd = 0.0f;
	}
	else
	{
		// Count time since sprint stopped
		TimeSinceSprintEnd += DeltaTime;

		// Recharge stamina if delay passed
		if (TimeSinceSprintEnd >= StaminaRechargeDelay)
		{
			StaminaPercent += StaminaRechargeRate * DeltaTime;
			StaminaPercent = FMath::Clamp(StaminaPercent, 0.0f, 1.0f);
		}
	}
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// Look
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookVertical", this, &AFPSCharacter::AddControllerPitchInput);

	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::EndJump);

	// Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::StopSprinting);

	// Interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSCharacter::Interact);

	// Pause
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AFPSCharacter::TogglePause);
}

void AFPSCharacter::MoveForward(float value)
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, value);
	}
}

void AFPSCharacter::MoveRight(float value)
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		FVector Direction = GetActorRightVector();
		AddMovementInput(Direction, value);
	}
}

void AFPSCharacter::StartJump()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		bPressedJump = true;
	}
}

void AFPSCharacter::EndJump()
{
	bPressedJump = false;
}

void AFPSCharacter::StartSprinting()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		// Only sprint if stamina available
		if (StaminaPercent > 0.0f)
		{
			bIsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			TimeSinceSprintEnd = 0.0f;
		}
	}
}

void AFPSCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	// Start counting delay before recharge
	TimeSinceSprintEnd = 0.0f;
}

// Simplified Interact function - now delegates to InteractionManager
void AFPSCharacter::Interact()
{
	if (InteractionManager)
	{
		AActor* Target = InteractionManager->GetCurrentInteractable();
		InteractionManager->PerformInteraction(this, Target);
	}
}

void AFPSCharacter::TogglePause()
{
	bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());

	if (bCurrentlyPaused)
	{
		// Resume game
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}

		// Just hide the widget instead of removing/destroying it
		if (PauseMenuWidget)
		{
			PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		// Pause game
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeUIOnly());
			PC->bShowMouseCursor = true;
		}

		// Create the widget if it doesn't exist
		if (!PauseMenuWidget && PauseMenuWidgetClass)
		{
			PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
			if (PauseMenuWidget)
			{
				PauseMenuWidget->AddToViewport();
			}
		}
		else if (PauseMenuWidget)
		{
			// Just show it again if it's already created
			PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}