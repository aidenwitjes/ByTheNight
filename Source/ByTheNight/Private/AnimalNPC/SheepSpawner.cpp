#include "AnimalNPC/SheepSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Algo/RandomShuffle.h"

// Sets default values
ASheepSpawner::ASheepSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASheepSpawner::BeginPlay()
{
    Super::BeginPlay();
    SpawnSheep();
}

void ASheepSpawner::SpawnSheep()
{
    TArray<AActor*> FoundZones;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnZoneTag, FoundZones);

    if (FoundZones.Num() == 0 || !SheepClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid zones found or SheepClass is null."));
        return;
    }

    // Shuffle zones
    Algo::RandomShuffle(FoundZones);

    int32 SpawnCount = FMath::Min(NumberToSpawn, FoundZones.Num());

    for (int32 i = 0; i < SpawnCount; ++i)
    {
        AActor* Zone = FoundZones[i];
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector Location = Zone->GetActorLocation();
        FRotator Rotation = Zone->GetActorRotation();

        GetWorld()->SpawnActor<AActor>(SheepClass, Location, Rotation, Params);
    }
}