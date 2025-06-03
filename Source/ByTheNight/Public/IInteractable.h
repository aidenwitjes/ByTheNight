// IInteractable.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

class AFPSCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class BYTHENIGHT_API IInteractable
{
    GENERATED_BODY()

public:
    // Called when player interacts with this object
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AFPSCharacter* Player);
    virtual void Interact_Implementation(AFPSCharacter* Player) {}

    // Returns the interaction prompt text to display
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FString GetInteractionPrompt() const;
    virtual FString GetInteractionPrompt_Implementation() const { return TEXT("Press [E] to interact"); }

    // Returns whether this object can currently be interacted with
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    bool CanInteract(AFPSCharacter* Player) const;
    virtual bool CanInteract_Implementation(AFPSCharacter* Player) const { return true; }

    // Returns the color for the highlight/debug visualization
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FLinearColor GetHighlightColor() const;
    virtual FLinearColor GetHighlightColor_Implementation() const { return FLinearColor::Yellow; }
};