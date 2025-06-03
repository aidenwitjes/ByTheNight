#include "AnimalNPC/Sheep.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Character/FPSCharacter.h"

ASheep::ASheep()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SheepMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SheepMesh"));
    SheepMesh->SetupAttachment(RootComponent);
    SheepMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SheepMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    AmbientAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbientAudio"));
    AmbientAudioComponent->SetupAttachment(RootComponent);
    AmbientAudioComponent->bAutoActivate = false;
    AmbientAudioComponent->bOverrideAttenuation = false;
    AmbientAudioComponent->bIsUISound = false;
    AmbientAudioComponent->bAllowSpatialization = true;

    FVector RandomOffset(FMath::FRandRange(-20.f, 20.f), FMath::FRandRange(-20.f, 20.f), 0.f);
    AmbientAudioComponent->SetRelativeLocation(RandomOffset);
}

void ASheep::BeginPlay()
{
    Super::BeginPlay();

    if (AmbientSheepSound && AmbientAudioComponent)
    {
        ScheduleNextBleat();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sheep missing AmbientSheepSound or AudioComponent"));
    }
}

void ASheep::Interact_Implementation(AFPSCharacter* Player)
{
    if (bIsCollected || !Player) return;

    bIsCollected = true;

    if (InteractionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, InteractionSound, GetActorLocation());
    }

    if (AmbientAudioComponent)
    {
        AmbientAudioComponent->Stop();
    }

    GetWorld()->GetTimerManager().ClearTimer(BleatTimerHandle);

    SheepMesh->SetVisibility(false);
    SetActorEnableCollision(false);

    UE_LOG(LogTemp, Log, TEXT("Sheep collected through IInteractable interface by %s!"), *Player->GetName());
}

FString ASheep::GetInteractionPrompt_Implementation() const
{
    return bIsCollected ? TEXT("") : InteractionPromptText;
}

bool ASheep::CanInteract_Implementation(AFPSCharacter* Player) const
{
    return !bIsCollected && Player != nullptr;
}

FLinearColor ASheep::GetHighlightColor_Implementation() const
{
    return bIsCollected ? FLinearColor::Gray : HighlightColor;
}

void ASheep::ScheduleNextBleat()
{
    if (bIsCollected || !AmbientSheepSound) return;

    const float Delay = FMath::FRandRange(MinBleatDelay, MaxBleatDelay);
    GetWorld()->GetTimerManager().SetTimer(BleatTimerHandle, this, &ASheep::PlayRandomBleat, Delay, false);
}

void ASheep::PlayRandomBleat()
{
    if (bIsCollected || !AmbientSheepSound || !AmbientAudioComponent) return;

    APawn* Player = GetPlayer();
    if (!Player) return;

    float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
    float Volume = CalculateVolumeFromDistance(Distance);

    if (Volume > 0.01f)
    {
        AmbientAudioComponent->SetSound(AmbientSheepSound);
        AmbientAudioComponent->SetVolumeMultiplier(Volume * FMath::FRandRange(0.8f, 1.2f));
        AmbientAudioComponent->SetPitchMultiplier(FMath::FRandRange(0.9f, 1.1f));
        AmbientAudioComponent->Play();

        UE_LOG(LogTemp, Log, TEXT("Sheep bleated with pitch %.2f, volume %.2f"),
            AmbientAudioComponent->PitchMultiplier, AmbientAudioComponent->VolumeMultiplier);
    }

    ScheduleNextBleat();
}

float ASheep::CalculateVolumeFromDistance(float Distance) const
{
    if (Distance <= MinAudioDistance)
    {
        return MaxVolume;
    }

    if (Distance >= MaxAudioDistance)
    {
        return 0.0f;
    }

    float DistanceRatio = (MaxAudioDistance - Distance) / (MaxAudioDistance - MinAudioDistance);
    return FMath::Clamp(DistanceRatio * MaxVolume, 0.0f, MaxVolume);
}

APawn* ASheep::GetPlayer() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    APlayerController* PC = World->GetFirstPlayerController();
    return PC ? PC->GetPawn() : nullptr;
}
