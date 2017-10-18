// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayBase.h"
#include "SceneViewExtension.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Engine/Public/IStereoLayers.h"


class APlayerController;
class FSceneView;
class FSceneViewFamily;
class UCanvas;

/**
 * Simple Head Mounted Display
 */
class FSimpleHMD : public FHeadMountedDisplayBase, public IStereoLayers, public ISceneViewExtension, public TSharedFromThis<FSimpleHMD, ESPMode::ThreadSafe>
{
	friend class FSplash;

public:
	/** IHeadMountedDisplay interface */
	virtual FName GetDeviceName() const override
	{
		static FName DefaultName(TEXT("SimpleHMD"));
		return DefaultName;
	}

	virtual bool IsHMDConnected() override { return true; }
	virtual bool IsHMDEnabled() const override;
	virtual void EnableHMD(bool allow = true) override;
	virtual EHMDDeviceType::Type GetHMDDeviceType() const override;
	virtual bool GetHMDMonitorInfo(MonitorInfo&) override;

	virtual void GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const override;

	virtual bool DoesSupportPositionalTracking() const override;
	virtual bool HasValidTrackingPosition() override;
	virtual void GetPositionalTrackingCameraProperties(FVector& OutOrigin, FQuat& OutOrientation, float& OutHFOV, float& OutVFOV, float& OutCameraDistance, float& OutNearPlane, float& OutFarPlane) const override;
	virtual void RebaseObjectOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation) const override;

	virtual void SetInterpupillaryDistance(float NewInterpupillaryDistance) override;
	virtual float GetInterpupillaryDistance() const override;

	virtual void GetCurrentOrientationAndPosition(FQuat& CurrentOrientation, FVector& CurrentPosition) override;
	virtual TSharedPtr<class ISceneViewExtension, ESPMode::ThreadSafe> GetViewExtension() override;
	virtual void ApplyHmdRotation(APlayerController* PC, FRotator& ViewRotation) override;
	virtual bool UpdatePlayerCamera(FQuat& CurrentOrientation, FVector& CurrentPosition) override;

	virtual bool IsChromaAbCorrectionEnabled() const override;

	virtual bool IsPositionalTrackingEnabled() const override;

	virtual bool IsHeadTrackingAllowed() const override;

	virtual void ResetOrientationAndPosition(float yaw = 0.f) override;
	virtual void ResetOrientation(float Yaw = 0.f) override;
	virtual void ResetPosition() override;

	virtual void SetClippingPlanes(float NCP, float FCP) override;

	virtual void SetBaseRotation(const FRotator& BaseRot) override;
	virtual FRotator GetBaseRotation() const override;

	virtual void SetBaseOrientation(const FQuat& BaseOrient) override;
	virtual FQuat GetBaseOrientation() const override;

	virtual void DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) override;

	virtual bool HasHiddenAreaMesh() const override;
	virtual bool HasVisibleAreaMesh() const override;

	/** IStereoRendering interface */
	virtual bool IsStereoEnabled() const override;
	virtual bool EnableStereo(bool stereo = true) override;
	virtual void AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const override;
	virtual void CalculateStereoViewOffset(const EStereoscopicPass StereoPassType, const FRotator& ViewRotation,
		const float MetersToWorld, FVector& ViewLocation) override;
	virtual FMatrix GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType, const float FOV) const override;
	virtual void InitCanvasFromView(FSceneView* InView, UCanvas* Canvas) override;
	virtual void GetEyeRenderParams_RenderThread(const struct FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const override;

	virtual bool NeedReAllocateViewportRenderTarget(const FViewport& Viewport) override;
	virtual bool ShouldUseSeparateRenderTarget() const override;
	virtual void RenderTexture_RenderThread(class FRHICommandListImmediate& RHICmdList, class FRHITexture2D* BackBuffer, class FRHITexture2D* SrcTexture) const override;
	virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 InTexFlags, uint32 InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override;
	virtual void CalculateRenderTargetSize(const FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) override;

	/** ISceneViewExtension interface */
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) {}
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;

	// IStereoLayers interface
	virtual uint32 CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual void DestroyLayer(uint32 LayerId) override;
	virtual void SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual bool GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc) override;
	virtual void MarkTextureForUpdate(uint32 LayerId) override;
	virtual void UpdateSplashScreen() override;

public:
	/** Constructor */
	FSimpleHMD();

	/** Destructor */
	virtual ~FSimpleHMD();

	/** @return	True if the HMD was initialized OK */
	bool IsInitialized();

private:

	FQuat					CurHmdOrientation;
	FQuat					LastHmdOrientation;

	FRotator				DeltaControlRotation;    // same as DeltaControlOrientation but as rotator
	FQuat					DeltaControlOrientation; // same as DeltaControlRotation but as quat

	double					LastSensorTime;

	void GetCurrentPose(FQuat& CurrentOrientation);

	// Spectator Screen
public:
	virtual FIntRect GetFullFlatEyeRect(FTexture2DRHIRef EyeTexture) const override;
	virtual void CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FTexture2DRHIParamRef SrcTexture, FIntRect SrcRect, FTexture2DRHIParamRef DstTexture, FIntRect DstRect, bool bClearBlack) const override;
	ESpectatorScreenMode GetSpectatorScreenMode() const;

private:
	void CreateSpectatorScreenController();
	bool ShouldDisableHiddenAndVisibileAreaMeshForSpectatorScreen_RenderThread() const;


};

