// GameOverGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameOverGameMode.generated.h"

class UUserWidget;

UCLASS()
class BYTHENIGHT_API AGameOverGameMode : public AGameModeBase
{
    GENERATED_BODY()
protected:
    virtual void StartPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    UPROPERTY()
    UUserWidget* GameOverWidget;
};