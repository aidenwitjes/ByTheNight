// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Logging/StructuredLog.h"
#include "FPSProjectGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BYTHENIGHT_API AFPSProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
};
