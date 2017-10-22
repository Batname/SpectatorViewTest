// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpectatorViewTestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Engine/GameEngine.h"
#include "SGameLayerManager.h"
#include "Slate/SceneViewport.h"
#include "Runtime/Slate/Public/Widgets/Layout/SConstraintCanvas.h"
#include "Runtime/Slate/Public/Widgets/SWeakWidget.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"

//////////////////////////////////////////////////////////////////////////
// ASpectatorViewTestCharacter

ASpectatorViewTestCharacter::ASpectatorViewTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASpectatorViewTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpectatorViewTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpectatorViewTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASpectatorViewTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASpectatorViewTestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASpectatorViewTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASpectatorViewTestCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASpectatorViewTestCharacter::OnResetVR);
}


void ASpectatorViewTestCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASpectatorViewTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASpectatorViewTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASpectatorViewTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASpectatorViewTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASpectatorViewTestCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASpectatorViewTestCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void ASpectatorViewTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	return;

	if (this->GetWorld()->WorldType == EWorldType::Game)
		StandaloneGame = true;
	else
		StandaloneGame = false;

	ExtraWindow = SNew(SWindow)
		.AutoCenter(EAutoCenter::None)
		.Title(FText::FromString(TEXT("Control Window")))
		.IsInitiallyMaximized(false)
		.ScreenPosition(FVector2D(0, 0))
		.ClientSize(FVector2D(1920, 1080))
		.CreateTitleBar(true)
		.SizingRule(ESizingRule::UserSized)
		.SupportsMaximize(false)
		.SupportsMinimize(true)
		.HasCloseButton(true)
		.UseOSWindowBorder(false);

	FSlateApplication & SlateApp = FSlateApplication::Get();

	SlateApp.AddWindow(ExtraWindow.ToSharedRef(), true);

	if (CustomUserWidgetBP)
	{
		//Creating our widget and adding it to our viewport
		CustomUserWidget = CreateWidget<UCustomUserWidget>(GetWorld(), CustomUserWidgetBP);

		//CustomUserWidget->AddToViewport();

		TSharedPtr<SWidget> UserSlateWidget = CustomUserWidget->TakeWidget();
		TSharedRef<SConstraintCanvas> ViewportWidget = SNew(SConstraintCanvas);

		ViewportWidget->AddSlot()
			[
				UserSlateWidget.ToSharedRef()
			];


		ViewportWidget->SetVisibility(EVisibility::Visible);

		ExtraWindow->SetContent(ViewportWidget);
		ExtraWindow->ShowWindow();

	}
}

void ASpectatorViewTestCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	return;

	if (ExtraWindow.Get() != nullptr)
	{
		if (StandaloneGame == false)
			ExtraWindow->RequestDestroyWindow();
		else
			ExtraWindow->DestroyWindowImmediately();
	}
}