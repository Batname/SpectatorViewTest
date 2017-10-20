// Fill out your copyright notice in the Description page of Project Settings.

#include "Private/SimpleHMD.h"
#include "SpectatorGameInstance.h"
#include "EngineModule.h"

void USpectatorGameInstance::InitializeMainUIContext(APlayerController* PlayerController)
{
	// Creates the world context. This should be the only WorldContext that ever gets created for this GameInstance.
	WorldContext = &GetEngine()->CreateNewWorldContext(EWorldType::None);
	WorldContext->OwningGameInstance = this;
	//WorldContext->RunAsDedicated = true;

	// create a SpectatorUI world
	UWorld* SpectatorUIWorld = UWorld::CreateWorld(EWorldType::None, false, "MainUI");

	SpectatorUIWorld->SetGameInstance(this);
	GEngine->DestroyWorldContext(SpectatorUIWorld);
	//WorldContext->SetCurrentWorld( SpectatorUIWorld );

	SpectatorUIWorld->AddController(PlayerController);

	Init();
}
