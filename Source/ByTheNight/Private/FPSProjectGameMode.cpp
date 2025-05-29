// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectGameMode.h"
#include "Blueprint/UserWidget.h"
#include "UGameHUDWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "CharacterObjects/Lantern.h"

void AFPSProjectGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	// 1. To print to screen
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameModeBase!"));

	// 2. To print to console
	// UE_LOG(LogTemp, Warning, TEXT("Hello World, this is FPSGameModeBase!")); // cout << <<

	// 3. Printing values
	// int TestNumber = 10;
	// FString Name("TestName");
	// bool BoolTest = true;
	// UE_LOG(LogTemp, Warning, TEXT("TestNumber: %i, Name: %s, BoolTest: %d"), TestNumber, *Name, BoolTest);

	// 4. Modern way of debugging values
	// UE_LOGFMT(LogTemp, Warning, "TestNumber: {0}, Name: {1}, BoolTest: {2}", TestNumber, Name, BoolTest);

	// Create and display HUD widget
	if (HUDWidgetClass)
	{
		HUDWidget = Cast<UGameHUDWidget>(CreateWidget(GetWorld(), HUDWidgetClass));
		if (HUDWidgetClass)
		{
			HUDWidget = CreateWidget<UGameHUDWidget>(GetWorld(), HUDWidgetClass);
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUDWidgetClass not set in GameMode!"));
		}
	}
	// Find lantern and bind to delegate
	ALantern* Lantern = Cast<ALantern>(UGameplayStatics::GetActorOfClass(this, ALantern::StaticClass()));
	if (Lantern)
	{
		Lantern->OnFuelDepletedDelegate.AddDynamic(this, &AFPSProjectGameMode::HandleLanternFuelDepleted);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lantern not found in scene!"));
	}
}
	void AFPSProjectGameMode::HandleLanternFuelDepleted()
	{
		UE_LOG(LogTemp, Warning, TEXT("Lantern fuel depleted! Triggering end scene..."));

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC && PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->StartCameraFade(0.f, 1.f, 1.0f, FLinearColor::Black, false, true);

			// Delay 1 second then trigger end scene
			FTimerHandle FadeTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, [this]()
				{
					UGameplayStatics::OpenLevel(this, "GameOver");
				}, 2.0f, false);
		}
	}