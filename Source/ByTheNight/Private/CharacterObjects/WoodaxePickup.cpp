#include "CharacterObjects/WoodaxePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/FPSCharacter.h"

AWoodaxePickup::AWoodaxePickup()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    WoodaxeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WoodaxeMesh"));
    WoodaxeMesh->SetupAttachment(RootComponent);

    WoodaxeMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    WoodaxeMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AWoodaxePickup::HandlePickup()
{
    if (bIsCollected) return;

    bIsCollected = true;

    if (InteractionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, InteractionSound, GetActorLocation());
    }

    WoodaxeMesh->SetVisibility(false);
    SetActorEnableCollision(false);
}

void AWoodaxePickup::Interact_Implementation(AFPSCharacter* Player)
{
    if (CanInteract_Implementation(Player))
    {
        HandlePickup();
    }
}

FString AWoodaxePickup::GetInteractionPrompt_Implementation() const
{
    return bIsCollected ? TEXT("") : InteractionPromptText;
}

bool AWoodaxePickup::CanInteract_Implementation(AFPSCharacter* Player) const
{
    return !bIsCollected && Player != nullptr;
}

FLinearColor AWoodaxePickup::GetHighlightColor_Implementation() const
{
    return bIsCollected ? FLinearColor::Gray : HighlightColor;
}
