// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UGameHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class BYTHENIGHT_API UGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void SetSheepCount(int32 Current, int32 Max);

    UFUNCTION(BlueprintCallable)
    void SetStamina(float Percent);

    UFUNCTION(BlueprintCallable)
    void SetFuel(float Percent);

    UFUNCTION(BlueprintCallable)
    void SetGameTime(float TimeRemaining);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SheepText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* StaminaBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* FuelBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* GameTimerText;
};
