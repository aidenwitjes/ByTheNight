// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class ALantern;
class ASheep;
class UGameHUDWidget;
class UPauseMenuWidget;

UCLASS()
class BYTHENIGHT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Base movement speeds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 600.0f;

	// Stamina values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaPercent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate = 0.20f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRechargeRate = 0.10f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRechargeDelay = 2.0f;

	float TimeSinceSprintEnd = 0.0f;
	bool bIsSprinting = false;

	// Lantern Blueprint class
	UPROPERTY(EditDefaultsOnly, Category = "Lantern")
	TSubclassOf<ALantern> LanternClass;

	// Lantern instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lantern")
	ALantern* Lantern;

	// Lantern anchor scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lantern")
	USceneComponent* LanternAnchor;

	// Editable offset from camera to place lantern
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
	FVector LanternAnchorOffset = FVector(50.f, 20.f, -10.f);

	// Editable rotation relative to camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
	FRotator LanternAnchorRotation = FRotator::ZeroRotator;

	// Gameplay values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 CurrentSheepCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 MaxSheepCount = 6;

	// Pause Menu
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	UPauseMenuWidget* PauseMenuWidget;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// Forward movement
	UFUNCTION()
	void MoveForward(float value);

	// Lateral movement
	UFUNCTION()
	void MoveRight(float value);

	// Jump motion
	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void EndJump();

	// Sprint motion
	UFUNCTION()
	void StartSprinting();

	UFUNCTION()
	void StopSprinting();

	// Interact action
	UFUNCTION()
	void Interact();

	// Check for interactables in crosshair
	UFUNCTION()
	void CheckForInteractables();

	// Pause functionality
	UFUNCTION()
	void TogglePause();
};