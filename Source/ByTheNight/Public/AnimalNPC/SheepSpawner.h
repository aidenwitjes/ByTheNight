// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SheepSpawner.generated.h"

UCLASS()
class BYTHENIGHT_API ASheepSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASheepSpawner();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Sheep Spawning")
    FName SpawnZoneTag = "SheepZone";

    UPROPERTY(EditAnywhere, Category = "Sheep Spawning")
    TSubclassOf<AActor> SheepClass;

    UPROPERTY(EditAnywhere, Category = "Sheep Spawning")
    int32 NumberToSpawn = 6;

    void SpawnSheep();
};
