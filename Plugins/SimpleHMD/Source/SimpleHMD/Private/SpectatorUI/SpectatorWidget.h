// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameEngine.h"
#include "SGameLayerManager.h"
#include "Slate/SceneViewport.h"
#include "SpectatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class USpectatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
	USpectatorWidget(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectatorUI")
	bool bSpectatorUICreated;
	
	UGameViewportClient* SpectatorUIViewportClient;
	TSharedPtr<FSceneViewport> SpectatorUISceneViewport;


	UFUNCTION(BlueprintCallable, Category = "SpectatorUI")
	bool CreateSpectatorUIWindow();

	UFUNCTION(BlueprintCallable, Category = "SpectatorUI")
	void UpdateSpectatorUI(float DeltaSeconds);

	void OnSpectatorUIWindowClosed(const TSharedRef<SWindow>& WindowBeingClosed);
};
