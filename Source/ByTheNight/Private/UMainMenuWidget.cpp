// Fill out your copyright notice in the Description page of Project Settings.


#include "UMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (PlayButton)
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);

    return true;
}

void UMainMenuWidget::OnPlayClicked()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        // Switch to game-only input mode
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    // Example: Open your main game level by name
    UGameplayStatics::OpenLevel(this, TEXT("LevelMap"));
}

void UMainMenuWidget::OnQuitClicked()
{
    // Quit the game
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
