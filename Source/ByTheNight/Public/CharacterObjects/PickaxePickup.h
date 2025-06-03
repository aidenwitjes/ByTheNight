#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "Sound/SoundCue.h"
#include "PickaxePickup.generated.h"

class UStaticMeshComponent;
class AFPSCharacter;

UCLASS()
class BYTHENIGHT_API APickaxePickup : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    APickaxePickup();

    // IInteractable interface
    virtual void Interact_Implementation(AFPSCharacter* Player) override;
    virtual FString GetInteractionPrompt_Implementation() const override;
    virtual bool CanInteract_Implementation(AFPSCharacter* Player) const override;
    virtual FLinearColor GetHighlightColor_Implementation() const override;

protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* PickaxeMesh;

    // Configurable data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FString InteractionPromptText = TEXT("Press [E] to pick up pickaxe");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FLinearColor HighlightColor = FLinearColor::Blue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* InteractionSound;

private:
    // State
    bool bIsCollected = false;

    // Helper
    void HandlePickup();
};
