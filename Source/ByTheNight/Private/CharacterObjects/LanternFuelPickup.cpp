// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterObjects/LanternFuelPickup.h"

// Sets default values
ALanternFuelPickup::ALanternFuelPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALanternFuelPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALanternFuelPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

