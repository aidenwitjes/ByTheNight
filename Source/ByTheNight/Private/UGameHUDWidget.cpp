// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // Optional: Initialize values here
}

void UGameHUDWidget::SetSheepCount(int32 Current, int32 Max)
{
    if (SheepText)
    {
        SheepText->SetText(FText::FromString(FString::Printf(TEXT("Sheep: %d/%d"), Current, Max)));
    }
}

void UGameHUDWidget::SetStamina(float Percent)
{
    if (StaminaBar)
    {
        StaminaBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
    }
}

void UGameHUDWidget::SetFuel(float Percent)
{
    if (FuelBar)
    {
        FuelBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
    }
}

void UGameHUDWidget::SetGameTime(float TimeRemaining)
{
    if (GameTimerText)
    {
        int32 Minutes = FMath::FloorToInt(TimeRemaining / 60.0f);
        int32 Seconds = FMath::FloorToInt(FMath::Fmod(TimeRemaining, 60.0f));
        FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        GameTimerText->SetText(FText::FromString(TimeString));
    }
}