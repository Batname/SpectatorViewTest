// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SimpleHMD.h"
#include "Misc/App.h"
#include "Modules/ModuleManager.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "ISimpleHMDPlugin.h"
#include "Runtime/Renderer/Private/SceneRendering.h"
#include "Runtime/Renderer/Private/PostProcess/PostProcessHMD.h"

//---------------------------------------------------
// SimpleHMD Plugin Implementation
//---------------------------------------------------

class FSimpleHMDPlugin : public ISimpleHMDPlugin
{
	/** IHeadMountedDisplayModule implementation */
	virtual TSharedPtr< class IHeadMountedDisplay, ESPMode::ThreadSafe > CreateHeadMountedDisplay() override;

	FString GetModuleKeyName() const override
	{
		return FString(TEXT("SimpleHMD"));
	}
};

IMPLEMENT_MODULE( FSimpleHMDPlugin, SimpleHMD )

TSharedPtr< class IHeadMountedDisplay, ESPMode::ThreadSafe > FSimpleHMDPlugin::CreateHeadMountedDisplay()
{
	TSharedPtr< FSimpleHMD, ESPMode::ThreadSafe > SimpleHMD( new FSimpleHMD() );
	if( SimpleHMD->IsInitialized() )
	{
		return SimpleHMD;
	}
	return NULL;
}


//---------------------------------------------------
// SimpleHMD IHeadMountedDisplay Implementation
//---------------------------------------------------

bool FSimpleHMD::IsHMDEnabled() const
{
	return true;
}

void FSimpleHMD::EnableHMD(bool enable)
{
}

EHMDDeviceType::Type FSimpleHMD::GetHMDDeviceType() const
{
	return EHMDDeviceType::DT_ES2GenericStereoMesh;
}

bool FSimpleHMD::GetHMDMonitorInfo(MonitorInfo& MonitorDesc)
{
	MonitorDesc.MonitorName = "";
	MonitorDesc.MonitorId = 0;
	MonitorDesc.DesktopX = MonitorDesc.DesktopY = MonitorDesc.ResolutionX = MonitorDesc.ResolutionY = 0;
	return false;
}

void FSimpleHMD::GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const
{
	OutHFOVInDegrees = 0.0f;
	OutVFOVInDegrees = 0.0f;
}

bool FSimpleHMD::DoesSupportPositionalTracking() const
{
	return false;
}

bool FSimpleHMD::HasValidTrackingPosition()
{
	return false;
}

void FSimpleHMD::GetPositionalTrackingCameraProperties(FVector& OutOrigin, FQuat& OutOrientation, float& OutHFOV, float& OutVFOV, float& OutCameraDistance, float& OutNearPlane, float& OutFarPlane) const
{
}

void FSimpleHMD::RebaseObjectOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation) const
{
}

void FSimpleHMD::SetInterpupillaryDistance(float NewInterpupillaryDistance)
{
}

float FSimpleHMD::GetInterpupillaryDistance() const
{
	return 0.064f;
}

void FSimpleHMD::GetCurrentPose(FQuat& CurrentOrientation)
{
	// very basic.  no head model, no prediction, using debuglocalplayer
	ULocalPlayer* Player = GEngine->GetDebugLocalPlayer();

	if (Player != NULL && Player->PlayerController != NULL)
	{
		FVector RotationRate = Player->PlayerController->GetInputVectorKeyState(EKeys::RotationRate);

		double CurrentTime = FApp::GetCurrentTime();
		double DeltaTime = 0.0;

		if (LastSensorTime >= 0.0)
		{
			DeltaTime = CurrentTime - LastSensorTime;
		}

		LastSensorTime = CurrentTime;

		// mostly incorrect, but we just want some sensor input for testing
		RotationRate *= DeltaTime;
		CurrentOrientation *= FQuat(FRotator(FMath::RadiansToDegrees(-RotationRate.X), FMath::RadiansToDegrees(-RotationRate.Y), FMath::RadiansToDegrees(-RotationRate.Z)));
	}
	else
	{
		CurrentOrientation = FQuat(FRotator(0.0f, 0.0f, 0.0f));
	}
}

void FSimpleHMD::GetCurrentOrientationAndPosition(FQuat& CurrentOrientation, FVector& CurrentPosition)
{
	CurrentPosition = FVector(0.0f, 0.0f, 0.0f);

	GetCurrentPose(CurrentOrientation);
	CurHmdOrientation = LastHmdOrientation = CurrentOrientation;
}

TSharedPtr<ISceneViewExtension, ESPMode::ThreadSafe> FSimpleHMD::GetViewExtension()
{
	TSharedPtr<FSimpleHMD, ESPMode::ThreadSafe> ptr(AsShared());
	return StaticCastSharedPtr<ISceneViewExtension>(ptr);
}

void FSimpleHMD::ApplyHmdRotation(APlayerController* PC, FRotator& ViewRotation)
{
	ViewRotation.Normalize();

	GetCurrentPose(CurHmdOrientation);
	LastHmdOrientation = CurHmdOrientation;

	const FRotator DeltaRot = ViewRotation - PC->GetControlRotation();
	DeltaControlRotation = (DeltaControlRotation + DeltaRot).GetNormalized();

	// Pitch from other sources is never good, because there is an absolute up and down that must be respected to avoid motion sickness.
	// Same with roll.
	DeltaControlRotation.Pitch = 0;
	DeltaControlRotation.Roll = 0;
	DeltaControlOrientation = DeltaControlRotation.Quaternion();

	ViewRotation = FRotator(DeltaControlOrientation * CurHmdOrientation);
}

bool FSimpleHMD::UpdatePlayerCamera(FQuat& CurrentOrientation, FVector& CurrentPosition)
{
	return false;
}

bool FSimpleHMD::IsChromaAbCorrectionEnabled() const
{
	return false;
}

bool FSimpleHMD::IsPositionalTrackingEnabled() const
{
	return false;
}

bool FSimpleHMD::IsHeadTrackingAllowed() const
{
	return true;
}

void FSimpleHMD::ResetOrientationAndPosition(float yaw)
{
	ResetOrientation(yaw);
	ResetPosition();
}

void FSimpleHMD::ResetOrientation(float Yaw)
{
}
void FSimpleHMD::ResetPosition()
{
}

void FSimpleHMD::SetClippingPlanes(float NCP, float FCP)
{
}

void FSimpleHMD::SetBaseRotation(const FRotator& BaseRot)
{
}

FRotator FSimpleHMD::GetBaseRotation() const
{
	return FRotator::ZeroRotator;
}

void FSimpleHMD::SetBaseOrientation(const FQuat& BaseOrient)
{
}

FQuat FSimpleHMD::GetBaseOrientation() const
{
	return FQuat::Identity;
}

void FSimpleHMD::DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize)
{
	float ClipSpaceQuadZ = 0.0f;
	FMatrix QuadTexTransform = FMatrix::Identity;
	FMatrix QuadPosTransform = FMatrix::Identity;
	const FSceneView& View = Context.View;
	const FIntRect SrcRect = View.ViewRect;

	FRHICommandListImmediate& RHICmdList = Context.RHICmdList;
	const FSceneViewFamily& ViewFamily = *(View.Family);
	FIntPoint ViewportSize = ViewFamily.RenderTarget->GetSizeXY();
	RHICmdList.SetViewport(0, 0, 0.0f, ViewportSize.X, ViewportSize.Y, 1.0f);

	static const uint32 NumVerts = 8;
	static const uint32 NumTris = 4;

	static const FDistortionVertex Verts[8] =
	{
		// left eye
		{ FVector2D(-0.9f, -0.9f), FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(-0.1f, -0.9f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(-0.1f, 0.9f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), 1.0f, 0.0f },
		{ FVector2D(-0.9f, 0.9f), FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), 1.0f, 0.0f },
		// right eye
		{ FVector2D(0.1f, -0.9f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(0.9f, -0.9f), FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(0.9f, 0.9f), FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f), 1.0f, 0.0f },
		{ FVector2D(0.1f, 0.9f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), 1.0f, 0.0f },
	};

	static const uint16 Indices[12] = { /*Left*/ 0, 1, 2, 0, 2, 3, /*Right*/ 4, 5, 6, 4, 6, 7 };

	DrawIndexedPrimitiveUP(Context.RHICmdList, PT_TriangleList, 0, NumVerts, NumTris, &Indices,
		sizeof(Indices[0]), &Verts, sizeof(Verts[0]));
}

bool FSimpleHMD::HasHiddenAreaMesh() const
{
	if (IsInRenderingThread())
	{
		if (ShouldDisableHiddenAndVisibileAreaMeshForSpectatorScreen_RenderThread())
		{
			return false;
		}
	}

	return true;
}

bool FSimpleHMD::HasVisibleAreaMesh() const
{
	if (IsInRenderingThread())
	{
		if (ShouldDisableHiddenAndVisibileAreaMeshForSpectatorScreen_RenderThread())
		{
			return false;
		}
	}

	return true;
}

bool FSimpleHMD::IsStereoEnabled() const
{
	return true;
}

bool FSimpleHMD::EnableStereo(bool stereo)
{
	return true;
}

void FSimpleHMD::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const
{
	SizeX = SizeX / 2;
	if( StereoPass == eSSP_RIGHT_EYE )
	{
		X += SizeX;
	}
}

void FSimpleHMD::CalculateStereoViewOffset(const enum EStereoscopicPass StereoPassType, const FRotator& ViewRotation, const float WorldToMeters, FVector& ViewLocation)
{
	if( StereoPassType != eSSP_FULL)
	{
		float EyeOffset = 3.20000005f;
		const float PassOffset = (StereoPassType == eSSP_LEFT_EYE) ? EyeOffset : -EyeOffset;
		ViewLocation += ViewRotation.Quaternion().RotateVector(FVector(0,PassOffset,0));
	}
}

FMatrix FSimpleHMD::GetStereoProjectionMatrix(const enum EStereoscopicPass StereoPassType, const float FOV) const
{
	const float ProjectionCenterOffset = 0.151976421f;
	const float PassProjectionOffset = (StereoPassType == eSSP_LEFT_EYE) ? ProjectionCenterOffset : -ProjectionCenterOffset;

	const float HalfFov = 2.19686294f / 2.f;
	const float InWidth = 640.f;
	const float InHeight = 480.f;
	const float XS = 1.0f / tan(HalfFov);
	const float YS = InWidth / tan(HalfFov) / InHeight;

	const float InNearZ = GNearClippingPlane;
	return FMatrix(
		FPlane(XS,                      0.0f,								    0.0f,							0.0f),
		FPlane(0.0f,					YS,	                                    0.0f,							0.0f),
		FPlane(0.0f,	                0.0f,								    0.0f,							1.0f),
		FPlane(0.0f,					0.0f,								    InNearZ,						0.0f))

		* FTranslationMatrix(FVector(PassProjectionOffset,0,0));
}

void FSimpleHMD::InitCanvasFromView(FSceneView* InView, UCanvas* Canvas)
{
}

void FSimpleHMD::GetEyeRenderParams_RenderThread(const FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const
{
	EyeToSrcUVOffsetValue = FVector2D::ZeroVector;
	EyeToSrcUVScaleValue = FVector2D(1.0f, 1.0f);
}

void FSimpleHMD::CalculateRenderTargetSize(const FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY)
{
	InOutSizeX = 500;
	InOutSizeY = 500;
}

bool FSimpleHMD::NeedReAllocateViewportRenderTarget(const FViewport& Viewport)
{
	//check(GRenderingThread && !GIsRenderingThreadSuspended);
	UE_LOG(LogTemp, Warning, TEXT("FSimpleHMD::NeedReAllocateViewportRenderTarget"));


	FIntPoint ViewportSize = Viewport.GetSizeXY();
	const FIntPoint RenderTargetSize = Viewport.GetRenderTargetTextureSizeXY();

	uint32 NewSizeX = ViewportSize.X;
	uint32 NewSizeY = ViewportSize.Y;
	CalculateRenderTargetSize(Viewport, NewSizeX, NewSizeY);

	return (NewSizeX != RenderTargetSize.X || NewSizeY != RenderTargetSize.Y);
}

bool FSimpleHMD::ShouldUseSeparateRenderTarget() const
{
	//check(GRenderingThread && !GIsRenderingThreadSuspended);

	return true;
}

void FSimpleHMD::RenderTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* BackBuffer, FRHITexture2D* SrcTexture) const
{
	if (SpectatorScreenController)
	{
		SpectatorScreenController->RenderSpectatorScreen_RenderThread(RHICmdList, BackBuffer, SrcTexture);
	}
}

bool FSimpleHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 InTexFlags, uint32 InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples)
{
	// Only called when RenderThread is suspended.  Both of these checks should pass.
	//check(GRenderingThread && !GIsRenderingThreadSuspended);

	return false;
}



void FSimpleHMD::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
	InViewFamily.EngineShowFlags.MotionBlur = 0;
	InViewFamily.EngineShowFlags.HMDDistortion = true;
	InViewFamily.EngineShowFlags.SetScreenPercentage(true);
	InViewFamily.EngineShowFlags.StereoRendering = IsStereoEnabled();
}

void FSimpleHMD::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
	InView.BaseHmdOrientation = FQuat(FRotator(0.0f,0.0f,0.0f));
	InView.BaseHmdLocation = FVector(0.f);
//	WorldToMetersScale = InView.WorldToMetersScale;
	InViewFamily.bUseSeparateRenderTarget = false;
}

void FSimpleHMD::PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
	check(IsInRenderingThread());

	if (SpectatorScreenController)
	{
		SpectatorScreenController->UpdateSpectatorScreenMode_RenderThread();
	}
}

void FSimpleHMD::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily)
{
	check(IsInRenderingThread());
}

FSimpleHMD::FSimpleHMD() :
	CurHmdOrientation(FQuat::Identity),
	LastHmdOrientation(FQuat::Identity),
	DeltaControlRotation(FRotator::ZeroRotator),
	DeltaControlOrientation(FQuat::Identity),
	LastSensorTime(-1.0)
{
}

FSimpleHMD::~FSimpleHMD()
{
}

bool FSimpleHMD::IsInitialized()
{
	CreateSpectatorScreenController();


	UE_LOG(LogTemp, Warning, TEXT(">> FSimpleHMD::IsInitialized()"));

	return true;
}

uint32 FSimpleHMD::CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc)
{
	return 1;
}

void FSimpleHMD::DestroyLayer(uint32 LayerId)
{

}


void FSimpleHMD::SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc)
{

}


bool FSimpleHMD::GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc)
{
	return true;
}


void FSimpleHMD::MarkTextureForUpdate(uint32 LayerId)
{

}


void FSimpleHMD::UpdateSplashScreen()
{

}
