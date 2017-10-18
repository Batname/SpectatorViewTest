// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpectatorViewTestGameMode.h"
#include "SpectatorViewTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpectatorViewTestGameMode::ASpectatorViewTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
