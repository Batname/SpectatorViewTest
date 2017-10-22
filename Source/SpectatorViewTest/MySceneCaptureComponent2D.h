// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MySceneCaptureComponent2D.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent))
class SPECTATORVIEWTEST_API UMySceneCaptureComponent2D : public USceneCaptureComponent2D
{
	GENERATED_BODY()
	
public:
	UMySceneCaptureComponent2D(const FObjectInitializer& ObjectInitializer);
	
};
