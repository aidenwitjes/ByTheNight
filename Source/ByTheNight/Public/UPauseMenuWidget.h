#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UPauseMenuWidget.generated.h"

class UButton;

UCLASS()
class BYTHENIGHT_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnReturnToMenuClicked();

    UFUNCTION()
    void OnQuitClicked();

    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ReturnToMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;
};
