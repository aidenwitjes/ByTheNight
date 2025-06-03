#include "CharacterObjects/PickaxePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/FPSCharacter.h"

APickaxePickup::APickaxePickup()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    PickaxeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickaxeMesh"));
    PickaxeMesh->SetupAttachment(RootComponent);

    PickaxeMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PickaxeMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void APickaxePickup::HandlePickup()
{
    if (bIsCollected) return;

    bIsCollected = true;

    if (InteractionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, InteractionSound, GetActorLocation());
    }

    PickaxeMesh->SetVisibility(false);
    SetActorEnableCollision(false);
}

void APickaxePickup::Interact_Implementation(AFPSCharacter* Player)
{
    if (CanInteract_Implementation(Player))
    {
        HandlePickup();
    }
}

FString APickaxePickup::GetInteractionPrompt_Implementation() const
{
    return bIsCollected ? TEXT("") : InteractionPromptText;
}

bool APickaxePickup::CanInteract_Implementation(AFPSCharacter* Player) const
{
    return !bIsCollected && Player != nullptr;
}

FLinearColor APickaxePickup::GetHighlightColor_Implementation() const
{
    return bIsCollected ? FLinearColor::Gray : HighlightColor;
}
