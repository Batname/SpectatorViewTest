// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayBase.h"
#include "SceneViewExtension.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Engine/Public/IStereoLayers.h"
#include "Runtime/RHI/Public/RHIResources.h"

#include "Slate/SGameLayerManager.h"
#include "Slate/SceneViewport.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/Slate/Public/Widgets/SViewport.h"

// Bat
#include "Engine/GameEngine.h"
#include "SGameLayerManager.h"
#include "Slate/SceneViewport.h"
#include "Runtime/Slate/Public/Widgets/Layout/SConstraintCanvas.h"
#include "Runtime/Slate/Public/Widgets/SWeakWidget.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/RenderCore/Public/RendererInterface.h"

class APlayerController;
class FSceneView;
class FSceneViewFamily;
class UCanvas;
class FSimpleHMD;
class IRendererModule;



class FSimpleHMDCustomPresent : public FRHICustomPresent
{
public:

	FSimpleHMDCustomPresent(FSimpleHMD* HMD);
	virtual ~FSimpleHMDCustomPresent();

	void Shutdown();

private:

	FSimpleHMD* HMD;

	bool bNeedResizeGVRRenderTarget;
	bool bSkipPresent;

public:

	/**
	* Allocates a render target texture.
	*
	* @param Index			(in) index of the buffer, changing from 0 to GetNumberOfBufferedFrames()
	* @return				true, if texture was allocated; false, if the default texture allocation should be used.
	*/
	bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumLayers, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags);

	// Frame operations
	void UpdateRenderingViewportList();
	void UpdateRenderingPose();
	void UpdateViewport(const FViewport& Viewport, FRHIViewport* ViewportRHI);

	void BeginRendering();
	void FinishRendering();

public:

	void CreateGVRSwapChain();
	///////////////////////////////////////
	// Begin FRHICustomPresent Interface //
	///////////////////////////////////////

	// Called when viewport is resized.
	virtual void OnBackBufferResize() override;

	// @param InOutSyncInterval - in out param, indicates if vsync is on (>0) or off (==0).
	// @return	true if normal Present should be performed; false otherwise. If it returns
	// true, then InOutSyncInterval could be modified to switch between VSync/NoVSync for the normal Present.
	virtual bool Present(int32& InOutSyncInterval) override;

	//// Called when rendering thread is acquired
	//virtual void OnAcquireThreadOwnership() {}

	//// Called when rendering thread is released
	//virtual void OnReleaseThreadOwnership() {}
};

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

	virtual bool HasHiddenAreaMesh() const override { return false; }
	virtual bool HasVisibleAreaMesh() const override { return false; }

	virtual bool OnStartGameFrame(FWorldContext& WorldContext) override;

	virtual bool OnEndGameFrame(FWorldContext& WorldContext) override;

	virtual void OnBeginPlay(FWorldContext& InWorldContext) override;
	virtual void OnEndPlay(FWorldContext& InWorldContext) override;


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
	virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 InTexFlags, uint32 InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override { return false; }
	virtual void CalculateRenderTargetSize(const FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) override;
	virtual void UpdateViewport(bool bUseSeparateRenderTarget, const class FViewport& Viewport, class SViewport* = nullptr) override;
	virtual void RenderTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FTexture2DRHIParamRef BackBuffer, FTexture2DRHIParamRef SrcTexture) const override;



	/** ISceneViewExtension interface */
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;

	// IStereoLayers interface
	virtual uint32 CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc) override { return 1; }
	virtual void DestroyLayer(uint32 LayerId) override {}
	virtual void SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc) override {}
	virtual bool GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc) override { return false; }
	virtual void MarkTextureForUpdate(uint32 LayerId) override {}
	virtual void UpdateSplashScreen() override {}


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

public:
	FSimpleHMDCustomPresent* CustomPresent;

// BAT
public:
	bool bIsRunning;

	TSharedPtr<FSceneViewport> SceneViewport1 = nullptr;
	TSharedPtr<SWindow> ExtraWindow1 = nullptr;
	TSharedPtr<SOverlay> ViewportOverlayWidget1 = nullptr;
	TSharedPtr<SViewport> MyViewport1 = nullptr;

	TSharedPtr<FSceneViewport> SceneViewport2 = nullptr;
	TSharedPtr<SWindow> ExtraWindow2 = nullptr;
	TSharedPtr<SOverlay> ViewportOverlayWidget2 = nullptr;
	TSharedPtr<SViewport> MyViewport2 = nullptr;

	bool StandaloneGame = false;

	IRendererModule* RendererModule;


// SpectatorScreen
private:
	void CreateSpectatorScreenController();
public:
	virtual FIntRect GetFullFlatEyeRect(FTexture2DRHIRef EyeTexture) const override;
	virtual void CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FTexture2DRHIParamRef SrcTexture, FIntRect SrcRect, FTexture2DRHIParamRef DstTexture, FIntRect DstRect, bool bClearBlack) const override;


// Steam
	void GetWindowBounds(int32* X, int32* Y, uint32* Width, uint32* Height);
	bool bStereoDesired;
	bool bStereoEnabled;
};

