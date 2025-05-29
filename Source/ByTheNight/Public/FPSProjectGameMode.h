// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Logging/StructuredLog.h"
#include "FPSProjectGameMode.generated.h"

class UGameHUDWidget;

UCLASS()
class BYTHENIGHT_API AFPSProjectGameMode : public AGameModeBase
{
    GENERATED_BODY()

protected:
    virtual void StartPlay() override;

    // Assign this in the editor to your UGameHUDWidget Blueprint class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UGameHUDWidget> HUDWidgetClass;

private:
    // Instance of the widget
    UPROPERTY()
    UGameHUDWidget* HUDWidget;

public:
    // Public getter for HUDWidget
    UGameHUDWidget* GetHUDWidget() const { return HUDWidget; }

    UFUNCTION()
    void HandleLanternFuelDepleted();
};