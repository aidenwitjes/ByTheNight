// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectGameMode.h"

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
}