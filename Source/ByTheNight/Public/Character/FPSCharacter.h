// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

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

	// Base movement speeds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 1200.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
};
