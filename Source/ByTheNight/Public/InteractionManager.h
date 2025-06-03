// InteractionManager.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionManager.generated.h"

class AFPSCharacter;
class IInteractable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BYTHENIGHT_API UInteractionManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionManager();

	// Main interaction functions
	void PerformInteraction(AFPSCharacter* Player, AActor* Target);
	void CheckForInteractables(AFPSCharacter* Player);

	// Getters for current interaction state
	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetCurrentInteractable() const { return CurrentInteractable; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	FString GetInteractionPrompt() const { return InteractionPrompt; }

protected:
	virtual void BeginPlay() override;

	// Current interaction state
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractable = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	FString InteractionPrompt = TEXT("");

	// Interaction settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 300.0f;

private:
	// Specific interaction handlers
	void HandleSheepInteraction(class ASheep* Sheep, AFPSCharacter* Player);
	void HandlePickaxeInteraction(class APickaxePickup* Pickaxe, AFPSCharacter* Player);
	void HandleWoodaxeInteraction(class AWoodaxePickup* Woodaxe, AFPSCharacter* Player);
	void HandleBoulderInteraction(class ABoulder* Boulder, AFPSCharacter* Player);
	void HandleTreeStumpInteraction(class ATreeStump* TreeStump, AFPSCharacter* Player);
	void HandleGenericInteraction(AActor* Actor, AFPSCharacter* Player);

	// Helper functions
	bool CanPlayerInteract(AFPSCharacter* Player) const;
	void DisplayInteractionFeedback(const FString& Message, const FLinearColor& Color) const;
};