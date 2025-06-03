// InteractionManager.cpp
#include "InteractionManager.h"
#include "Character/FPSCharacter.h"
#include "IInteractable.h"
#include "AnimalNPC/Sheep.h"
#include "CharacterObjects/PickaxePickup.h"
#include "CharacterObjects/WoodaxePickup.h"
#include "Barriers/TreeStump.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

UInteractionManager::UInteractionManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionManager::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionManager::PerformInteraction(AFPSCharacter* Player, AActor* Target)
{
	if (!Player)
	{
		DisplayInteractionFeedback(TEXT("No player."), FLinearColor::Red);
		return;
	}

	if (!Target)
	{
		DisplayInteractionFeedback(TEXT("No target to interact with."), FLinearColor::Red);
		return;
	}

	IInteractable* InteractableObject = Cast<IInteractable>(Target);
	if (!InteractableObject)
	{
		DisplayInteractionFeedback(TEXT("Object doesn't implement interactable."), FLinearColor::Red);
		return;
	}

	// Interact regardless of CanInteract (let handler decide)
	InteractableObject->Execute_Interact(Target, Player);

	// Specific logic
	if (ASheep* Sheep = Cast<ASheep>(Target))
	{
		HandleSheepInteraction(Sheep, Player);
	}
	else if (APickaxePickup* Pickaxe = Cast<APickaxePickup>(Target))
	{
		HandlePickaxeInteraction(Pickaxe, Player);
	}
	else if (AWoodaxePickup* Woodaxe = Cast<AWoodaxePickup>(Target))
	{
		HandleWoodaxeInteraction(Woodaxe, Player);
	}
	else if (ABoulder* Boulder = Cast<ABoulder>(Target))
	{
		HandleBoulderInteraction(Boulder, Player);
	}
	else if (ATreeStump* TreeStump = Cast<ATreeStump>(Target))
	{
		HandleTreeStumpInteraction(TreeStump, Player);
	}
	else
	{
		HandleGenericInteraction(Target, Player);
	}
}

void UInteractionManager::CheckForInteractables(AFPSCharacter* Player)
{
	if (!CanPlayerInteract(Player))
		return;

	// Clear previous interaction data
	CurrentInteractable = nullptr;
	InteractionPrompt = TEXT("");

	// Raycast from camera to detect interactables
	FVector Start = Player->FPSCameraComponent->GetComponentLocation();
	FVector End = Start + (Player->FPSCameraComponent->GetForwardVector() * InteractionRange);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Player);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable* InteractableObject = Cast<IInteractable>(HitActor);
			if (InteractableObject)
			{
				CurrentInteractable = HitActor;

				if (InteractableObject->Execute_CanInteract(HitActor, Player))
				{
					InteractionPrompt = InteractableObject->Execute_GetInteractionPrompt(HitActor);
				}

				FLinearColor PromptColor = InteractableObject->Execute_CanInteract(HitActor, Player)
					? InteractableObject->Execute_GetHighlightColor(HitActor)
					: FLinearColor::Yellow;

				GEngine->AddOnScreenDebugMessage(1, 0.f,
					FColor(PromptColor.R * 255, PromptColor.G * 255, PromptColor.B * 255),
					InteractionPrompt);
			}
		}
	}
}

void UInteractionManager::HandleSheepInteraction(ASheep* Sheep, AFPSCharacter* Player)
{
	Player->CurrentSheepCount++;
	DisplayInteractionFeedback(TEXT("Collected a sheep!"), FLinearColor::Green);
}

void UInteractionManager::HandlePickaxeInteraction(APickaxePickup* Pickaxe, AFPSCharacter* Player)
{
	Player->PickaxeCount++;
	DisplayInteractionFeedback(TEXT("Picked up a pickaxe!"), FLinearColor::Blue);
}
void UInteractionManager::HandleWoodaxeInteraction(AWoodaxePickup* Woodaxe, AFPSCharacter* Player)
{
	Player->WoodaxeCount++;
	DisplayInteractionFeedback(TEXT("Picked up a woodaxe!"), FLinearColor::Blue);
}
void UInteractionManager::HandleBoulderInteraction(ABoulder* Boulder, AFPSCharacter* Player)
{
	if (Boulder->IsDestroyed())
		return;

	if (Player->HasPickaxe())
	{
		DisplayInteractionFeedback(TEXT("Boulder destroyed using a pickaxe!"), FLinearColor::Red);
	}
	else
	{
		DisplayInteractionFeedback(TEXT("You need a pickaxe to break this."), FLinearColor::Yellow);
	}
}
void UInteractionManager::HandleTreeStumpInteraction(ATreeStump* TreeStump, AFPSCharacter* Player)
{
	if (TreeStump->IsDestroyed())
		return;

	if (Player->HasWoodaxe())
	{
		DisplayInteractionFeedback(TEXT("Tree stump destroyed!"), FLinearColor::Red);
	}
	else
	{
		DisplayInteractionFeedback(TEXT("You need a woodaxe to break this."), FLinearColor::Yellow);
	}
}
void UInteractionManager::HandleGenericInteraction(AActor* Actor, AFPSCharacter* Player)
{
	DisplayInteractionFeedback(TEXT("Interacted with object!"), FLinearColor::Green);
}

bool UInteractionManager::CanPlayerInteract(AFPSCharacter* Player) const
{
	return Player && !UGameplayStatics::IsGamePaused(GetWorld());
}

void UInteractionManager::DisplayInteractionFeedback(const FString& Message, const FLinearColor& Color) const
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f,
		FColor(Color.R * 255, Color.G * 255, Color.B * 255), Message);
}