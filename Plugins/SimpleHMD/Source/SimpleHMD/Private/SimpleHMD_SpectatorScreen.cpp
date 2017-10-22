#include "SimpleHMD.h"

#include "PipelineStateCache.h"
#include "ClearQuad.h"
#include "DefaultSpectatorScreenController.h"
#include "Runtime/RHI/Public/RHIStaticStates.h"
#include "Runtime/ShaderCore/Public/GlobalShader.h"
#include "Runtime/Engine/Public/ScreenRendering.h"

void FSimpleHMD::CreateSpectatorScreenController()
{
	SpectatorScreenController = MakeUnique<FDefaultSpectatorScreenController>(this);
}

void FSimpleHMD::RenderTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FTexture2DRHIParamRef BackBuffer, FTexture2DRHIParamRef SrcTexture) const
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleHMD::RenderTexture_RenderThread"));

	// BAT
	check(IsInRenderingThread());
	check(SpectatorScreenController);
	SpectatorScreenController->RenderSpectatorScreen_RenderThread(RHICmdList, BackBuffer, SrcTexture);
}

FIntRect FSimpleHMD::GetFullFlatEyeRect(FTexture2DRHIRef EyeTexture) const
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleHMD::GetFullFlatEyeRect"));

	check(IsInRenderingThread());
	static FVector2D SrcNormRectMin(0.125f, 0.2f);
	static FVector2D SrcNormRectMax(0.375f, 0.8f);
	return FIntRect(EyeTexture->GetSizeX() * SrcNormRectMin.X, EyeTexture->GetSizeY() * SrcNormRectMin.Y, EyeTexture->GetSizeX() * SrcNormRectMax.X, EyeTexture->GetSizeY() * SrcNormRectMax.Y);
}

void FSimpleHMD::CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FTexture2DRHIParamRef SrcTexture, FIntRect SrcRect, FTexture2DRHIParamRef DstTexture, FIntRect DstRect, bool bClearBlack) const
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleHMD::CopyTexture_RenderThread"));


	check(IsInRenderingThread());

	const uint32 ViewportWidth = DstRect.Width();
	const uint32 ViewportHeight = DstRect.Height();
	const FIntPoint TargetSize(ViewportWidth, ViewportHeight);

	const float SrcTextureWidth = SrcTexture->GetSizeX();
	const float SrcTextureHeight = SrcTexture->GetSizeY();
	float U = 0.f, V = 0.f, USize = 1.f, VSize = 1.f;
	if (!SrcRect.IsEmpty())
	{
		U = SrcRect.Min.X / SrcTextureWidth;
		V = SrcRect.Min.Y / SrcTextureHeight;
		USize = SrcRect.Width() / SrcTextureWidth;
		VSize = SrcRect.Height() / SrcTextureHeight;
	}

	SetRenderTarget(RHICmdList, DstTexture, FTextureRHIRef());

	if (bClearBlack)
	{
		const FIntRect ClearRect(0, 0, DstTexture->GetSizeX(), DstTexture->GetSizeY());
		RHICmdList.SetViewport(ClearRect.Min.X, ClearRect.Min.Y, 0, ClearRect.Max.X, ClearRect.Max.Y, 1.0f);
		DrawClearQuad(RHICmdList, FLinearColor::Black);
	}

	RHICmdList.SetViewport(DstRect.Min.X, DstRect.Min.Y, 0, DstRect.Max.X, DstRect.Max.Y, 1.0f);


	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
	GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
	GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
	GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
	GraphicsPSOInit.PrimitiveType = PT_TriangleList;

	const auto FeatureLevel = GMaxRHIFeatureLevel;
	auto ShaderMap = GetGlobalShaderMap(FeatureLevel);

	TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
	TShaderMapRef<FScreenPS> PixelShader(ShaderMap);

	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = RendererModule->GetFilterVertexDeclaration().VertexDeclarationRHI;
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);

	SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

	const bool bSameSize = DstRect.Size() == SrcRect.Size();
	if (bSameSize)
	{
		PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Point>::GetRHI(), SrcTexture);
	}
	else
	{
		PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), SrcTexture);
	}

	RendererModule->DrawRectangle(
		RHICmdList,
		0, 0,
		ViewportWidth, ViewportHeight,
		U, V,
		USize, VSize,
		TargetSize,
		FIntPoint(1, 1),
		*VertexShader,
		EDRF_Default);
}