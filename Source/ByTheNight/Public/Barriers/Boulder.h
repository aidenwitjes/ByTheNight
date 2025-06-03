#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "Boulder.generated.h"

UCLASS()
class BYTHENIGHT_API ABoulder : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    ABoulder();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BoulderMesh;
    
    UPROPERTY(VisibleAnywhere, Category = "State")
    bool bIsDestroyed = false;

public:
    virtual void Tick(float DeltaTime) override;

    // IInteractable Interface
    virtual void Interact_Implementation(AFPSCharacter* Player) override;
    virtual FString GetInteractionPrompt_Implementation() const override;
    virtual bool CanInteract_Implementation(AFPSCharacter* Player) const override;
    virtual FLinearColor GetHighlightColor_Implementation() const override;

    // Optional getter
    bool IsDestroyed() const { return bIsDestroyed; }
};
