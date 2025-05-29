#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sheep.generated.h"

class UStaticMeshComponent;
class UAudioComponent;
class USoundCue;

UCLASS()
class BYTHENIGHT_API ASheep : public AActor
{
    GENERATED_BODY()

public:
    ASheep();

    virtual void BeginPlay() override;

    void Interact(AActor* Interactor);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SheepMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* AmbientAudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* AmbientSheepSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* InteractionSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "5000.0"))
    float MaxAudioDistance = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "5000.0"))
    float MinAudioDistance = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MaxVolume = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Bleat Timing", meta = (ClampMin = "0.1", ClampMax = "60.0"))
    float MinBleatDelay = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Bleat Timing", meta = (ClampMin = "0.1", ClampMax = "60.0"))
    float MaxBleatDelay = 7.0f;

private:
    bool bIsCollected = false;
    FTimerHandle BleatTimerHandle;

    void ScheduleNextBleat();
    void PlayRandomBleat();

    float CalculateVolumeFromDistance(float Distance) const;
    APawn* GetPlayer() const;
};
