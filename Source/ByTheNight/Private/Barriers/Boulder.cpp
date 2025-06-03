#include "Barriers/Boulder.h"
#include "Character/FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ABoulder::ABoulder()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create mesh component for visuals
    BoulderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoulderMesh"));
    RootComponent = BoulderMesh;
}

void ABoulder::BeginPlay()
{
    Super::BeginPlay();
}

void ABoulder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABoulder::Interact_Implementation(AFPSCharacter* Player)
{
    // Avoid re-triggering logic if already destroyed
    if (bIsDestroyed)
        return;

    if (Player && Player->HasPickaxe())
    {
        Player->UsePickaxe();
        bIsDestroyed = true;
        Destroy();
    }
}

FString ABoulder::GetInteractionPrompt_Implementation() const
{
    if (bIsDestroyed)
    {
        return TEXT("");
    }

    return TEXT("Press [E] to use a pickaxe and break the boulder");
}

bool ABoulder::CanInteract_Implementation(AFPSCharacter* Player) const
{
    return !bIsDestroyed && Player && Player->HasPickaxe();
}

FLinearColor ABoulder::GetHighlightColor_Implementation() const
{
    return FLinearColor::Red;
}