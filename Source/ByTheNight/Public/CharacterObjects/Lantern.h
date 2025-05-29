#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lantern.generated.h"

class UPointLightComponent;
class UStaticMeshComponent;

// Delegate declaration
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFuelDepletedDelegate);

UCLASS()
class BYTHENIGHT_API ALantern : public AActor
{
    GENERATED_BODY()

public:
    ALantern();

    virtual void Tick(float DeltaTime) override;

    // Refuel the lantern by a percentage (0.0 to 1.0)
    void RefillByPercent(float Percent);

    float GetFuelPercent() const;

    // ? Delegate for fuel depletion
    UPROPERTY(BlueprintAssignable, Category = "Lantern")
    FOnFuelDepletedDelegate OnFuelDepletedDelegate;

protected:
    virtual void BeginPlay() override;
    
    void DrainFuel(float DeltaTime);
    void UpdateLightState();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
    float MaxFuel = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
    float Fuel = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
    float FuelDrainRate = (1.0f / 90.0f); // A rate providing 90 seconds of fuel

    // Visuals
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* LanternMesh;

    UPROPERTY(VisibleAnywhere)
    UPointLightComponent* Light;

private:
    bool bHasTriggeredFuelDepletion = false;
};
