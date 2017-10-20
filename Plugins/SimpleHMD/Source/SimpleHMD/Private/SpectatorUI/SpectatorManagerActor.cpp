// Fill out your copyright notice in the Description page of Project Settings.

#include "Private/SimpleHMD.h"
#include "SpectatorManagerActor.h"


// Sets default values
ASpectatorManagerActor::ASpectatorManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpectatorManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpectatorManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

