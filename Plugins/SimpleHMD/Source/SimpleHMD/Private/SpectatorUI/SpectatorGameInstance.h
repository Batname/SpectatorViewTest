// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpectatorWidget.h"
#include "SpectatorManagerActor.h"
#include "SpectatorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class USpectatorGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectatorUI")
	USpectatorWidget* SpectatorWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectatorUI")
	ASpectatorManagerActor* SpectatorManagerActor;

	void InitializeMainUIContext(APlayerController* PlayerController);
	
	
};
