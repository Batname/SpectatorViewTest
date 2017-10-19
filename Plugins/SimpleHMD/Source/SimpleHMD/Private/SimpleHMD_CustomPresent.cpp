#include "SimpleHMD.h"

FSimpleHMDCustomPresent::FSimpleHMDCustomPresent(FSimpleHMD* HMD)
	: FRHICustomPresent(nullptr)
	, HMD(HMD)
{

}
FSimpleHMDCustomPresent::~FSimpleHMDCustomPresent()
{

}

void FSimpleHMDCustomPresent::Shutdown()
{

}

void FSimpleHMDCustomPresent::CreateGVRSwapChain()
{

}

void FSimpleHMDCustomPresent::OnBackBufferResize()
{

}

bool FSimpleHMDCustomPresent::Present(int32& InOutSyncInterval)
{
	return true;
}


bool FSimpleHMDCustomPresent::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumLayers, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags)
{
	return true;
}

// Frame operations
void FSimpleHMDCustomPresent::UpdateRenderingViewportList()
{

}

void FSimpleHMDCustomPresent::UpdateRenderingPose()
{

}

void FSimpleHMDCustomPresent::UpdateViewport(const FViewport& Viewport, FRHIViewport* ViewportRHI)
{

}

void FSimpleHMDCustomPresent::BeginRendering()
{

}


void FSimpleHMDCustomPresent::FinishRendering()
{

}