#include "Barriers/TreeStump.h"
#include "Character/FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ATreeStump::ATreeStump()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create mesh component for visuals
    TreeStumpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeStumpMesh"));
    RootComponent = TreeStumpMesh;

    // Initialize destroyed state
    bIsDestroyed = false;
}

void ATreeStump::BeginPlay()
{
    Super::BeginPlay();
}

void ATreeStump::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATreeStump::Interact_Implementation(AFPSCharacter* Player)
{
    // Avoid re-triggering logic if already destroyed
    if (bIsDestroyed)
        return;

    if (Player && Player->HasWoodaxe())
    {
        Player->UseWoodaxe();
        bIsDestroyed = true;
        Destroy();
    }
}

FString ATreeStump::GetInteractionPrompt_Implementation() const
{
    if (bIsDestroyed)
    {
        return TEXT("");
    }

    return TEXT("Press [E] to use a woodaxe and break the tree stump");
}

bool ATreeStump::CanInteract_Implementation(AFPSCharacter* Player) const
{
    return !bIsDestroyed && Player && Player->HasWoodaxe();
}

FLinearColor ATreeStump::GetHighlightColor_Implementation() const
{
    return FLinearColor::Red;
}
