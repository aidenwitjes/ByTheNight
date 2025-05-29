#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UGameOverWidget.generated.h"

class UButton;

UCLASS()
class BYTHENIGHT_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void OnReturnToMenuClicked();

    UFUNCTION()
    void OnQuitClicked();

    UFUNCTION()
    void OnRestartClicked();

    // Exposing buttons for Blueprints binding
    UPROPERTY(meta = (BindWidget))
    UButton* ReturnToMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    UButton* RestartButton;
};
