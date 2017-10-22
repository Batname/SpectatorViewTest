// Fill out your copyright notice in the Description page of Project Settings.

#include "MySceneCaptureComponent2D.h"



UMySceneCaptureComponent2D::UMySceneCaptureComponent2D(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("UMySceneCaptureComponent2D::UMySceneCaptureComponent2D"));

	CaptureStereoPass = EStereoscopicPass::eSSP_MONOSCOPIC_EYE;
}

