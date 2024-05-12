// Copyright Epic Games, Inc. All Rights Reserved.

#include "RepelTheUprisingCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "RTUPlayerState.h"
#include "../Components/RTUHealthComponent.h"
#include "../Components/RTUStaminaComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RepelTheUprising/Components/RTUFoodComponent.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"
#include "RepelTheUprising/Framework/RepelTheUprisingGameMode.h"
#include "RepelTheUprising/Widgets/RTUPlayerHUD.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARepelTheUprisingCharacter

ARepelTheUprisingCharacter::ARepelTheUprisingCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Set up defaults for the third person mesh 
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);

/** Components */
	InventoryComp = CreateDefaultSubobject<URTUInventoryComponent>(TEXT("Inventory Component"));
	HealthComp = CreateDefaultSubobject<URTUHealthComponent>(TEXT("Health Component"));
	StaminaComp = CreateDefaultSubobject<URTUStaminaComponent>(TEXT("Stamina Component"));
	FoodComp = CreateDefaultSubobject<URTUFoodComponent>(TEXT("Food Component"));
	
/** Variables */
	bIsCrouching = false;
}

void ARepelTheUprisingCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ReferenceDelayHandle, this, &ARepelTheUprisingCharacter::SetReferences, 0.5f, false, 0.5f);
/*
	if (PlayerHUDWidget)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		URTUPlayerHUD* PlayerWidgetR = CreateWidget<URTUPlayerHUD>(GetLocalViewingPlayerController(), PlayerHUDWidget);
		//PlayerWidgetRef->AddToViewport();
		if (PlayerWidgetR == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD Reference is invalid"));
		}
	}
*/
}

//////////////////////////////////////////////////////////////////////////// Input

void ARepelTheUprisingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARepelTheUprisingCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARepelTheUprisingCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARepelTheUprisingCharacter::StopSprint);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::ToggleCrouch);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::InteractWith);

		// Player Menu
		EnhancedInputComponent->BindAction(PlayerMenuAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::TogglePlayerWidget);
		

		// Test Action
		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::DoTestAction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARepelTheUprisingCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ARepelTheUprisingCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARepelTheUprisingCharacter::StartSprint(const FInputActionValue& Value)
{
	if (StaminaComp && !bIsCrouching)
	{
		StaminaComp->StartSprint();
	}
}

void ARepelTheUprisingCharacter::StopSprint(const FInputActionValue& Value)
{
	if (StaminaComp)
	{
		StaminaComp->StopSprint();
	}
}

void ARepelTheUprisingCharacter::ToggleCrouch(const FInputActionValue& Value)
{
	bIsCrouching = !bIsCrouching;

	if (bIsCrouching && StaminaComp)
	{
		Crouch();
		StaminaComp->StopSprint();
	}
	else
	{
		UnCrouch();
	}
}

void ARepelTheUprisingCharacter::DoTestAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, (TEXT("Do Test Action called")));
	HealthComp->RemoveSomeHealth();
}

void ARepelTheUprisingCharacter::SetReferences()
{
	PlayerStateRef = Cast<ARTUPlayerState>(GetPlayerState());
	if (!PlayerStateRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to Get Player State Ref"), *GetName());
	}
	GameModeRef = Cast<ARepelTheUprisingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to get Game Mode Ref"), *GetName());
	}

}

void ARepelTheUprisingCharacter::InteractWith()
{
	if (InventoryComp)
	{
		InventoryComp->InteractWithItem();
	}
}

void ARepelTheUprisingCharacter::TogglePlayerWidget()
{
	if (PlayerWidgetRef != nullptr)
	{
		if (!bInventoryIsShowing)
		{
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
				{
					if (MenuInputMapping)
					{
						InputSystem->RemoveMappingContext(DefaultInputMapping);
						InputSystem->AddMappingContext(MenuInputMapping, 0);
						UE_LOG(LogTemp, Warning, TEXT("New input mapping set"));
					}
				}

				PlayerWidgetRef->AddInventoryToHUD();
				PC->SetShowMouseCursor(true);
				bInventoryIsShowing = true;
			}
		}
		else
		{
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
				{
					if (MenuInputMapping)
					{
						InputSystem->RemoveMappingContext(MenuInputMapping);
						InputSystem->AddMappingContext(DefaultInputMapping, 0);
						UE_LOG(LogTemp, Warning, TEXT("New input mapping set"));
					}
				}

				PlayerWidgetRef->RemoveInventory();
				PC->SetShowMouseCursor(false);
				bInventoryIsShowing = false;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Widget Ref is not valid"));
	}
}
