// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacter.h"
#include "FPSProjectGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterObjects/Lantern.h"
#include "Kismet/GameplayStatics.h"
#include "UGameHUDWidget.h"

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
	
	// Display data for movement speed (testing sprinting logic)
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Speed: %f"), GetCharacterMovement()->Velocity.Size()));
	
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

}

void AFPSCharacter::MoveForward(float value)
{
	// 1. Unreal tutorial way
	// FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	FVector Direction = GetActorForwardVector();
	AddMovementInput(Direction, value);
}

void AFPSCharacter::MoveRight(float value)
{
	FVector Direction = GetActorRightVector();
	AddMovementInput(Direction, value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::EndJump()
{
	bPressedJump = false;
}

void AFPSCharacter::StartSprinting()
{
	// Only sprint if stamina available
	if (StaminaPercent > 0.0f)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		// Reset delay timer since sprint started
		TimeSinceSprintEnd = 0.0f;
	}
}

void AFPSCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	// Start counting delay before recharge
	TimeSinceSprintEnd = 0.0f;
}

void AFPSCharacter::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Interact Button Pressed!"));
}



