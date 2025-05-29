#include "UGameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ReturnToMenuButton)
        ReturnToMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnReturnToMenuClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnQuitClicked);

    if (RestartButton)
        RestartButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnRestartClicked);
}

void UGameOverWidget::OnReturnToMenuClicked()
{
    UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
}

void UGameOverWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UGameOverWidget::OnRestartClicked()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    UGameplayStatics::OpenLevel(this, TEXT("LevelMap"));
}
