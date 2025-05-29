// GameOverGameMode.cpp
#include "GameOverGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void AGameOverGameMode::StartPlay()
{
    Super::StartPlay();

    if (GameOverWidgetClass)
    {
        GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
        if (GameOverWidget)
        {
            GameOverWidget->AddToViewport();

            // Optional: Pause game input
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->SetShowMouseCursor(true);
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(GameOverWidget->TakeWidget());
                PC->SetInputMode(InputMode);
            }
        }
    }
}