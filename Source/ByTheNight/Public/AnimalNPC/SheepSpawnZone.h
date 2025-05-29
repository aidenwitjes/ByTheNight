#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SheepSpawnZone.generated.h"

class UBillboardComponent;

UCLASS()
class BYTHENIGHT_API ASheepSpawnZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASheepSpawnZone();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

    UPROPERTY(EditAnywhere, Category = "Sheep Zone")
    float Radius = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Sheep Zone")
    FColor DebugColor = FColor::Green;

    UPROPERTY(EditAnywhere, Category = "Sheep Zone")
    bool bShowDebugSphere = true;
};
