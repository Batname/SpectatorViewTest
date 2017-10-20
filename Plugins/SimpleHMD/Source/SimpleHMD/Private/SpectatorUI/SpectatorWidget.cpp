// Fill out your copyright notice in the Description page of Project Settings.

#include "Private/SimpleHMD.h"
#include "SpectatorWidget.h"
#include "SpectatorGameInstance.h"
#include "Runtime/Slate/Public/Widgets/Layout/SConstraintCanvas.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

USpectatorWidget::USpectatorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

bool USpectatorWidget::CreateSpectatorUIWindow()
{
	bSpectatorUICreated = false;
	if (GIsClient)
	{
		//Initialize GameInstance of new viewport client
		USpectatorGameInstance* SpectatorGameInstance = NewObject<USpectatorGameInstance>(GEngine, USpectatorGameInstance::StaticClass());
		SpectatorGameInstance->InitializeMainUIContext(GetWorld()->GetFirstPlayerController());

		//Initialize Viewport Client
		SpectatorUIViewportClient = NewObject<UGameViewportClient>(GEngine, UGameViewportClient::StaticClass());
		SpectatorUIViewportClient->Init(*SpectatorGameInstance->GetWorldContext(), SpectatorGameInstance);

		SpectatorGameInstance->GetWorldContext()->GameViewport = SpectatorUIViewportClient;

		if (SpectatorUIViewportClient)
		{
			UE_LOG(LogTemp, Warning, TEXT("CreateSpectatorUIWindow SpectatorUIViewportClient"));

			//create and register a new Window
			TSharedRef<SWindow> Window = SNew(SWindow)
				.ScreenPosition(FVector2D(100, 100))
				.ClientSize(FVector2D(1024, 512))
				.SizingRule(ESizingRule::UserSized)
				.Title(FText::FromString(TEXT("MainUI")))
				.AutoCenter(EAutoCenter::PrimaryWorkArea)
				.FocusWhenFirstShown(false)
				.UseOSWindowBorder(false)
				.CreateTitleBar(true)
				.SupportsTransparency(EWindowTransparency::PerWindow)
				.InitialOpacity(0.82f)
				.SupportsMaximize(true);
			FSlateApplication::Get().AddWindow(Window, false);
			Window->SetViewportSizeDrivenByWindow(false);
			Window->SetOnWindowClosed(FOnWindowClosed::CreateUObject(this, &USpectatorWidget::OnSpectatorUIWindowClosed));

			// Attach the viewport client to a new viewport.
			TSharedRef<SOverlay> ViewportOverlayWidgetRef = SNew(SOverlay);
			ViewportOverlayWidgetRef->SetCursor(EMouseCursor::Default);

			TSharedRef<SGameLayerManager> LayerManagerRef = SNew(SGameLayerManager)
				.SceneViewport(SpectatorUIViewportClient->GetGameViewport())
				.Visibility(EVisibility::Visible)
				[
					ViewportOverlayWidgetRef
				];

			TSharedPtr<class SViewport> Viewport = SNew(SViewport)
				.RenderDirectlyToWindow(true)
				.EnableStereoRendering(true)
				[
					LayerManagerRef
				];

			SpectatorUIViewportClient->SetViewportOverlayWidget(Window, ViewportOverlayWidgetRef);
			SpectatorUIViewportClient->SetGameLayerManager(LayerManagerRef);

			SpectatorUISceneViewport = MakeShareable(new FSceneViewport(SpectatorUIViewportClient, Viewport));
			SpectatorUIViewportClient->Viewport = SpectatorUISceneViewport.Get();
			Viewport->SetViewportInterface(SpectatorUISceneViewport.ToSharedRef());

			FViewportFrame* ViewportFrame = SpectatorUISceneViewport.Get();
			SpectatorUIViewportClient->SetViewportFrame(ViewportFrame);

			Window->SetContent(Viewport.ToSharedRef());

			TSharedPtr<SWidget> UserSlateWidget = TakeWidget();
			TSharedRef<SConstraintCanvas> ViewportWidget = SNew(SConstraintCanvas);

			ViewportWidget->AddSlot()
				.Offset(BIND_UOBJECT_ATTRIBUTE(FMargin, GetFullScreenOffset))
				[
					UserSlateWidget.ToSharedRef()
				];

			SpectatorUIViewportClient->AddViewportWidgetContent(ViewportWidget, 10);
			SpectatorUIViewportClient->SetCaptureMouseOnClick(EMouseCaptureMode::NoCapture);

			Window->ShowWindow();
		}
	}

	return false;
}

void USpectatorWidget::UpdateSpectatorUI(float DeltaSeconds)
{
	if (SpectatorUISceneViewport->HasMouseCapture())
		FSlateApplication::Get().ReleaseMouseCapture();


	if (!SpectatorUIViewportClient->GetWindow()->IsViewportSizeDrivenByWindow())
		SpectatorUIViewportClient->GetWindow()->SetViewportSizeDrivenByWindow(true);
}

void USpectatorWidget::OnSpectatorUIWindowClosed(const TSharedRef<SWindow>& WindowBeingClosed)
{
	if (!SpectatorUIViewportClient)
	{
		return;
	}

	SpectatorUIViewportClient->CloseRequested(SpectatorUISceneViewport->GetViewport());
	SpectatorUISceneViewport.Reset();

	SpectatorUIViewportClient = NULL;
	SpectatorUISceneViewport = NULL;

	UGameEngine* gameEngine = Cast<UGameEngine>(GEngine);
	if (gameEngine)
	{
		if (gameEngine->GameViewportWindow.IsValid())
		{
			gameEngine->OnGameWindowClosed(gameEngine->GameViewportWindow.Pin().ToSharedRef());
		}
	}

	bSpectatorUICreated = false;
}

