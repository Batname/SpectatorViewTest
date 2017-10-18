#include "SimpleHMD.h"

// 
class FSimpleSpectatorScreenController : public FDefaultSpectatorScreenController
{
public:
	FSimpleSpectatorScreenController(FSimpleHMD* InSimpleHMDDevice)
		: FDefaultSpectatorScreenController(InSimpleHMDDevice)
		, SimpleHMDDevice(InSimpleHMDDevice)
	{
	}

	void RenderSpectatorScreen_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* BackBuffer, FTexture2DRHIRef SrcTexture) const
	{
		UE_LOG(LogTemp, Warning, TEXT("FSimpleSpectatorScreenController::RenderSpectatorScreen_RenderThread()"));

		FDefaultSpectatorScreenController::RenderSpectatorScreen_RenderThread(RHICmdList, BackBuffer, SrcTexture);
	}

	void RenderSpectatorModeUndistorted(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef TargetTexture, FTexture2DRHIRef EyeTexture, FTexture2DRHIRef OtherTexture)
	{

	}

	void RenderSpectatorModeDistorted(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef TargetTexture, FTexture2DRHIRef EyeTexture, FTexture2DRHIRef OtherTexture)
	{
	}

	void RenderSpectatorModeSingleEye(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef TargetTexture, FTexture2DRHIRef EyeTexture, FTexture2DRHIRef OtherTexture)
	{
	}

private:
	FSimpleHMD* SimpleHMDDevice;
};

void FSimpleHMD::CreateSpectatorScreenController()
{
	SpectatorScreenController = MakeUnique<FSimpleSpectatorScreenController>(this);
}

bool FSimpleHMD::ShouldDisableHiddenAndVisibileAreaMeshForSpectatorScreen_RenderThread() const
{
	check(IsInRenderingThread());
	return false;
}

ESpectatorScreenMode FSimpleHMD::GetSpectatorScreenMode() const
{
	return SpectatorScreenController ? SpectatorScreenController->GetSpectatorScreenMode() : ESpectatorScreenMode::Disabled;
}

FIntRect FSimpleHMD::GetFullFlatEyeRect(FTexture2DRHIRef EyeTexture) const
{
	check(IsInRenderingThread());

	return FIntRect(0, 0, 500, 500);
}

void FSimpleHMD::CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FTexture2DRHIParamRef SrcTexture, FIntRect SrcRect, FTexture2DRHIParamRef DstTexture, FIntRect DstRect, bool bClearBlack) const
{
}