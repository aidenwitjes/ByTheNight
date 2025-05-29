#include "UPauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ResumeButton)
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);

    if (ReturnToMenuButton)
        ReturnToMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnReturnToMenuClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
}

void UPauseMenuWidget::OnResumeClicked()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        // Resume game time
        UGameplayStatics::SetGamePaused(GetWorld(), false);

        // Set back to game-only input
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    // Remove the pause menu from screen
    SetVisibility(ESlateVisibility::Hidden);
}

void UPauseMenuWidget::OnReturnToMenuClicked()
{
    UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
}

void UPauseMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
