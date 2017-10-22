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

#include "Runtime/SlateCore/Public/Rendering/SlateRenderer.h"
#include "Runtime/Engine/Public/UnrealEngine.h"

/** Helper function for acquiring the appropriate FSceneViewport */
FSceneViewport* FindSceneViewport()
{
	if (!GIsEditor)
	{
		UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
		return GameEngine->SceneViewport.Get();
	}
	return nullptr;
}

void FSimpleHMD::GetWindowBounds(int32* X, int32* Y, uint32* Width, uint32* Height)
{
	*X = 0;
	*Y = 0;
	*Width = 1920;
	*Height = 1080;
}


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

IMPLEMENT_MODULE(FSimpleHMDPlugin, SimpleHMD)

TSharedPtr< class IHeadMountedDisplay, ESPMode::ThreadSafe > FSimpleHMDPlugin::CreateHeadMountedDisplay()
{
	TSharedPtr< FSimpleHMD, ESPMode::ThreadSafe > SimpleHMD(new FSimpleHMD());
	if (SimpleHMD->IsInitialized())
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

bool FSimpleHMD::OnStartGameFrame(FWorldContext& WorldContext)
{
	if (CustomPresent)
	{
		CustomPresent->UpdateRenderingViewportList();
	}


	return true;
}

bool FSimpleHMD::OnEndGameFrame(FWorldContext& WorldContext)
{
	return true;
}

void FSimpleHMD::OnBeginPlay(FWorldContext& InWorldContext)
{

}
void FSimpleHMD::OnEndPlay(FWorldContext& InWorldContext)
{
	bIsRunning = false;
	// close the window
	if (ExtraWindow1.Get() != nullptr)
	{
		if (StandaloneGame == false)
			ExtraWindow1->RequestDestroyWindow();
		else
			ExtraWindow1->DestroyWindowImmediately();
	}

	if (ExtraWindow2.Get() != nullptr)
	{
		if (StandaloneGame == false)
			ExtraWindow2->RequestDestroyWindow();
		else
			ExtraWindow2->DestroyWindowImmediately();
	}
}

bool FSimpleHMD::IsStereoEnabled() const
{
	return true;
}

bool FSimpleHMD::EnableStereo(bool stereo)
{
	// Set the viewport to match that of the HMD display
	//FSceneViewport* SceneVP = FindSceneViewport();
	FSceneViewport* SceneVP = GEngine->GameViewport->GetGameViewport();
	UE_LOG(LogHMD, Error, TEXT("FSimpleHMD::EnableStereo %p"), SceneVP);

	if (SceneVP)
	{
		TSharedPtr<SWindow> Window = SceneVP->FindWindow();
		if (Window.IsValid() && SceneVP->GetViewportWidget().IsValid())
		{
			int32 ResX = 2160;
			int32 ResY = 1200;

			MonitorInfo MonitorDesc;
			if (GetHMDMonitorInfo(MonitorDesc))
			{
				ResX = MonitorDesc.ResolutionX;
				ResY = MonitorDesc.ResolutionY;
			}
			FSystemResolution::RequestResolutionChange(ResX, ResY, EWindowMode::WindowedFullscreen);

			if (stereo)
			{
				int32 PosX, PosY;
				uint32 Width, Height;
				GetWindowBounds(&PosX, &PosY, &Width, &Height);
				SceneVP->SetViewportSize(Width, Height);
				bStereoEnabled = bStereoDesired;
			}
			else
			{
				// Note: Setting before resize to ensure we don't try to allocate a new vr rt.
				bStereoEnabled = bStereoDesired;

				FRHIViewport* const ViewportRHI = SceneVP->GetViewportRHI();
				if (ViewportRHI != nullptr)
				{
					ViewportRHI->SetCustomPresent(nullptr);
				}

				FVector2D size = SceneVP->FindWindow()->GetSizeInScreen();
				SceneVP->SetViewportSize(size.X, size.Y);
				Window->SetViewportSizeDrivenByWindow(true);
			}
		}
	}

	// Uncap fps to enable FPS higher than 62
	GEngine->bForceDisableFrameRateSmoothing = bStereoEnabled;

	return bStereoEnabled;
}

void FSimpleHMD::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const
{
	SizeX = SizeX / 2;
	if (StereoPass == eSSP_RIGHT_EYE)
	{
		X += SizeX;
	}
}

void FSimpleHMD::CalculateStereoViewOffset(const enum EStereoscopicPass StereoPassType, const FRotator& ViewRotation, const float WorldToMeters, FVector& ViewLocation)
{
	if (StereoPassType != eSSP_FULL)
	{
		float EyeOffset = 3.20000005f;
		const float PassOffset = (StereoPassType == eSSP_LEFT_EYE) ? EyeOffset : -EyeOffset;
		ViewLocation += ViewRotation.Quaternion().RotateVector(FVector(0, PassOffset, 0));
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
		FPlane(XS, 0.0f, 0.0f, 0.0f),
		FPlane(0.0f, YS, 0.0f, 0.0f),
		FPlane(0.0f, 0.0f, 0.0f, 1.0f),
		FPlane(0.0f, 0.0f, InNearZ, 0.0f))

		* FTranslationMatrix(FVector(PassProjectionOffset, 0, 0));
}

void FSimpleHMD::InitCanvasFromView(FSceneView* InView, UCanvas* Canvas)
{
}

void FSimpleHMD::GetEyeRenderParams_RenderThread(const FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const
{
	if (Context.View.StereoPass == eSSP_LEFT_EYE)
	{
		EyeToSrcUVOffsetValue.X = 0.0f;
		EyeToSrcUVOffsetValue.Y = 0.0f;

		EyeToSrcUVScaleValue.X = 0.5f;
		EyeToSrcUVScaleValue.Y = 1.0f;
	}
	else
	{
		EyeToSrcUVOffsetValue.X = 0.5f;
		EyeToSrcUVOffsetValue.Y = 0.0f;

		EyeToSrcUVScaleValue.X = 0.5f;
		EyeToSrcUVScaleValue.Y = 1.0f;
	}
}

void FSimpleHMD::CalculateRenderTargetSize(const FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY)
{
	check(IsInGameThread());

	{
		static const auto CVar = IConsoleManager::Get().FindTConsoleVariableDataFloat(TEXT("r.ScreenPercentage"));
		float value = CVar->GetValueOnGameThread();
		if (value > 0.0f)
		{
			InOutSizeX = FMath::CeilToInt(InOutSizeX * value / 100.f);
			InOutSizeY = FMath::CeilToInt(InOutSizeY * value / 100.f);
		}
	}
}

bool FSimpleHMD::NeedReAllocateViewportRenderTarget(const FViewport& Viewport)
{
	//check(IsInGameThread());

	//UE_LOG(LogTemp, Warning, TEXT("FSimpleHMD::NeedReAllocateViewportRenderTarget"));


	//if (IsStereoEnabled())
	//{
	//	const uint32 InSizeX = Viewport.GetSizeXY().X;
	//	const uint32 InSizeY = Viewport.GetSizeXY().Y;
	//	FIntPoint RenderTargetSize;
	//	RenderTargetSize.X = Viewport.GetRenderTargetTexture()->GetSizeX();
	//	RenderTargetSize.Y = Viewport.GetRenderTargetTexture()->GetSizeY();

	//	uint32 NewSizeX = InSizeX, NewSizeY = InSizeY;
	//	CalculateRenderTargetSize(Viewport, NewSizeX, NewSizeY);
	//	if (NewSizeX != RenderTargetSize.X || NewSizeY != RenderTargetSize.Y)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

bool FSimpleHMD::ShouldUseSeparateRenderTarget() const
{
	check(IsInGameThread());
	return IsStereoEnabled();
}

void FSimpleHMD::UpdateViewport(bool bUseSeparateRenderTarget, const class FViewport& InViewport, class SViewport* ViewportWidget)
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleHMD::UpdateViewport"));

	check(IsInGameThread());

	if (GIsEditor && ViewportWidget)
	{
		if (!ViewportWidget->IsStereoRenderingAllowed())
		{
			return;
		}
	}

	FRHIViewport* const ViewportRHI = InViewport.GetViewportRHI().GetReference();
	if (!ViewportRHI)
	{
		return;
	}

	if (!IsStereoEnabled())
	{
		if ((!bUseSeparateRenderTarget || GIsEditor) && ViewportRHI)
		{
			ViewportRHI->SetCustomPresent(nullptr);
		}
		return;
	}

	if (CustomPresent)
	{
		CustomPresent->UpdateViewport(InViewport, ViewportRHI);
	}

	if (!bIsRunning)
	{
		bIsRunning = true;
		UE_LOG(LogHMD, Error, TEXT("bIsRunning"));

		//---------------------------------------------- WINDOW 1
		{
			ExtraWindow1 = SNew(SWindow)
				.ClientSize(FVector2D(1200, 800));

			TSharedRef<SWindow> SlateWinRef = ExtraWindow1.ToSharedRef();
			FSlateApplication & SlateApp = FSlateApplication::Get();
			SlateApp.AddWindow(SlateWinRef, true);

			//---------------- create viewport
			ViewportOverlayWidget1 = SNew(SOverlay);
			TSharedRef<SGameLayerManager> LayerManagerRef = SNew(SGameLayerManager)
				.SceneViewport(GEngine->GameViewport->GetGameViewport())
				.Visibility(EVisibility::Visible)
				.Cursor(EMouseCursor::None)
				[
					ViewportOverlayWidget1.ToSharedRef()
				];

			MyViewport1 = SNew(SViewport)
				.RenderDirectlyToWindow(false) // true crashes some stuff because HMDs need the rendertarget tex for distortion etc..
				.EnableGammaCorrection(false)
				.EnableStereoRendering(false) // not displaying on an HMD
				.Cursor(EMouseCursor::None)
				[
					LayerManagerRef
				];
			//---------------- create viewport

			FSceneViewport* SceneVP = GEngine->GameViewport->GetGameViewport();
			SceneViewport1 = MakeShareable(SceneVP, [](FSceneViewport* SceneVP)
			{
				UE_LOG(LogHMD, Error, TEXT("MakeShareable DELETE"));
			});
			MyViewport1->SetViewportInterface(SceneViewport1.ToSharedRef());
			ExtraWindow1->SetContent(MyViewport1.ToSharedRef());

			ExtraWindow1->ShowWindow();
		}
		//---------------------------------------------- WINDOW 1


		//---------------------------------------------- WINDOW 2
		{
			ExtraWindow2 = SNew(SWindow)
				.ClientSize(FVector2D(1200, 800));

			TSharedRef<SWindow> SlateWinRef = ExtraWindow2.ToSharedRef();
			FSlateApplication & SlateApp = FSlateApplication::Get();
			SlateApp.AddWindow(SlateWinRef, true);

			//---------------- create viewport
			ViewportOverlayWidget2 = SNew(SOverlay);
			TSharedRef<SGameLayerManager> LayerManagerRef = SNew(SGameLayerManager)
				.SceneViewport(GEngine->GameViewport->GetGameViewport())
				.Visibility(EVisibility::Visible)
				.Cursor(EMouseCursor::None)
				[
					ViewportOverlayWidget2.ToSharedRef()
				];

			MyViewport2 = SNew(SViewport)
				.RenderDirectlyToWindow(false) // true crashes some stuff because HMDs need the rendertarget tex for distortion etc..
				.EnableGammaCorrection(false)
				.EnableStereoRendering(false) // not displaying on an HMD
				.Cursor(EMouseCursor::None)
				[
					LayerManagerRef
				];
			//---------------- create viewport

			FSceneViewport* SceneVP = GEngine->GameViewport->GetGameViewport();
			SceneViewport2 = MakeShareable(SceneVP, [](FSceneViewport* SceneVP)
			{
				UE_LOG(LogHMD, Error, TEXT("MakeShareable DELETE"));
			});
			MyViewport2->SetViewportInterface(SceneViewport2.ToSharedRef());
			ExtraWindow2->SetContent(MyViewport2.ToSharedRef());

			ExtraWindow2->ShowWindow();
		}
		//---------------------------------------------- WINDOW 1

		if (GWorld->WorldType == EWorldType::Game)
			StandaloneGame = true;
		else
			StandaloneGame = false;
	}
}

void FSimpleHMD::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
	InViewFamily.EngineShowFlags.MotionBlur = 0;
	InViewFamily.EngineShowFlags.HMDDistortion = false;
	InViewFamily.EngineShowFlags.StereoRendering = IsStereoEnabled();
}

void FSimpleHMD::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
	InView.BaseHmdOrientation = FQuat(FRotator(0.0f, 0.0f, 0.0f));
	InView.BaseHmdLocation = FVector(0.f);
	InViewFamily.bUseSeparateRenderTarget = false;
}

void FSimpleHMD::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
	if (CustomPresent)
	{
		CustomPresent->UpdateRenderingPose();
	}
}

void FSimpleHMD::PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
	check(IsInRenderingThread());
}

void FSimpleHMD::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily)
{
	check(IsInRenderingThread());

	if (CustomPresent)
	{
		CustomPresent->BeginRendering();
	}

	// BAT
	check(SpectatorScreenController);
	SpectatorScreenController->UpdateSpectatorScreenMode_RenderThread();
}

FSimpleHMD::FSimpleHMD() :
	CurHmdOrientation(FQuat::Identity),
	LastHmdOrientation(FQuat::Identity),
	DeltaControlRotation(FRotator::ZeroRotator),
	DeltaControlOrientation(FQuat::Identity),
	LastSensorTime(-1.0),
	CustomPresent(nullptr),
	bIsRunning(false),
	ExtraWindow1(nullptr),
	ExtraWindow2(nullptr),
	bStereoDesired(false),
	bStereoEnabled(false)
{

	CustomPresent = new FSimpleHMDCustomPresent(this);

	UE_LOG(LogTemp, Warning, TEXT("FSimpleHMD::FSimpleHMD"));
}

FSimpleHMD::~FSimpleHMD()
{
	if (CustomPresent)
	{
		CustomPresent->Shutdown();
		CustomPresent = nullptr;
	}
}

bool FSimpleHMD::IsInitialized()
{
	// grab a pointer to the renderer module for displaying our mirror window
	static const FName RendererModuleName("Renderer");
	RendererModule = FModuleManager::GetModulePtr<IRendererModule>(RendererModuleName);

	// BAT
	CreateSpectatorScreenController();

	return true;
}