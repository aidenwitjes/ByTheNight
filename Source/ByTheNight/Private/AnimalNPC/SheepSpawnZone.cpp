#include "AnimalNPC/SheepSpawnZone.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASheepSpawnZone::ASheepSpawnZone()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add("SheepSpawnZone");
	SetActorHiddenInGame(true); // So it doesn't show in game
}

// Called when the game starts or when spawned
void ASheepSpawnZone::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

#if WITH_EDITOR
    if (bShowDebugSphere)
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, DebugColor, false, -1.0f, 0, 2.0f);
    }
#endif
}

