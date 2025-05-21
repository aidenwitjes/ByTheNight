#include "CharacterObjects/Lantern.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

ALantern::ALantern()
{
    PrimaryActorTick.bCanEverTick = true;

    LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));
    RootComponent = LanternMesh;

    Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
    Light->SetupAttachment(RootComponent);
    Light->SetIntensity(3000.0f);
    Light->SetVisibility(true);
}

void ALantern::BeginPlay()
{
    Super::BeginPlay();
    UpdateLightState();
}

void ALantern::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Fuel > 0.0f)
    {
        DrainFuel(DeltaTime);
    }
}

void ALantern::DrainFuel(float DeltaTime)
{
    Fuel -= FuelDrainRate * DeltaTime;
    Fuel = FMath::Clamp(Fuel, 0.0f, MaxFuel);
    UpdateLightState();
}

void ALantern::RefillByPercent(float Percent)
{
    if (Percent > 0.0f)
    {
        Fuel += MaxFuel * Percent;
        Fuel = FMath::Clamp(Fuel, 0.0f, MaxFuel);
        UpdateLightState();
    }
}

float ALantern::GetFuelPercent() const
{
    return Fuel / MaxFuel;
}

void ALantern::UpdateLightState()
{
    if (Light)
    {
        Light->SetVisibility(Fuel > 0.0f); // Light stays on only if fuel remains
    }
}
