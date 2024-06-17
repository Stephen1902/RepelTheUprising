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
#include "RepelTheUprising/Components/RTUFoodComponent.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"
#include "RepelTheUprising/Widgets/RTUInventorySlot.h"
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

	// The code is too fast for the object being referenced to be fully created.  Wait before setting references.
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

		// Actions
		EnhancedInputComponent->BindAction(StandardAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::StandardActionInGame);
		EnhancedInputComponent->BindAction(StandardAction, ETriggerEvent::Completed, this, &ARepelTheUprisingCharacter::StandardActionHUD);
		EnhancedInputComponent->BindAction(AlternateAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::AlternateActionInGame);

		// Dragging
		EnhancedInputComponent->BindAction(StandardDragAction, ETriggerEvent::Triggered, this, &ARepelTheUprisingCharacter::DragStandardHUD);
		EnhancedInputComponent->BindAction(AlternateDragAction, ETriggerEvent::Triggered, this, &ARepelTheUprisingCharacter::DragAlternateHUD);

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
	
}

void ARepelTheUprisingCharacter::StandardActionInGame(const FInputActionValue& Value)
{
	// These events only occur when the player HUD is not on screen 
	if (!bInventoryIsShowing && !bHUDIsShowing)
	{
		
	}
}

void ARepelTheUprisingCharacter::AlternateActionInGame(const FInputActionValue& Value)
{
	// These events only occur when the player HUD is not on screen 
	if (!bInventoryIsShowing && !bHUDIsShowing)
	{
		
	}
}

void ARepelTheUprisingCharacter::DragStandardHUD(const FInputActionValue& Value)
{
	// Dragging can only happen when the player's HUD is on screen
	if (bInventoryIsShowing || bHUDIsShowing)
	{
		
	}
}

void ARepelTheUprisingCharacter::DragAlternateHUD(const FInputActionValue& Value)
{
	// Dragging can only happen when the player's HUD is on screen
	if (bInventoryIsShowing || bHUDIsShowing)
	{
		
	}
}

void ARepelTheUprisingCharacter::StandardActionHUD(const FInputActionValue& Value)
{
	// If the player is not dragging something, show the information for this item
	if (!bIsDragging)
	{
		
	}
}

void ARepelTheUprisingCharacter::SetReferences()
{
	PlayerStateRef = Cast<ARTUPlayerState>(GetPlayerState());
	if (!PlayerStateRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to Get Player State Ref"), *GetName());
	}
}

void ARepelTheUprisingCharacter::InteractWith()
{
	if (bHUDIsShowing)
	{
		ChangeInputToGame();
		PlayerWidgetRef->RemoveCurrentWidget();
		bHUDIsShowing = false;
	}
	else if (InventoryComp)
	{
		InventoryComp->InteractWithItem();
	}
}

void ARepelTheUprisingCharacter::TogglePlayerWidget()
{
	if (PlayerWidgetRef != nullptr)
	{
		// Check if an existing HUD, container, trader inventory etc. is already on screen
		if (bHUDIsShowing)
		{
			PlayerWidgetRef->RemoveCurrentWidget();
			bHUDIsShowing = false;
		}
		
		if (!bInventoryIsShowing)
		{
				ChangeInputToUI();
				PlayerWidgetRef->AddInventoryToHUD();
				bInventoryIsShowing = true;
		}
		else
		{
				ChangeInputToGame();
				PlayerWidgetRef->RemoveCurrentWidget();
				bInventoryIsShowing = false;
			}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Widget Ref is not valid"));
	}
}

void ARepelTheUprisingCharacter::ChangeInputToUI()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		{
			if (MenuInputMapping)
			{
				InputSystem->RemoveMappingContext(DefaultInputMapping);
				InputSystem->AddMappingContext(MenuInputMapping, 99);
			}
		}

		PC->SetShowMouseCursor(true);
	}
}

void ARepelTheUprisingCharacter::ChangeInputToGame()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
 	{
 		UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
 		{
 			if (MenuInputMapping)
 			{
 				InputSystem->RemoveMappingContext(MenuInputMapping);
 				InputSystem->AddMappingContext(DefaultInputMapping, 0);
 			}
 		}
 
 		PC->SetShowMouseCursor(false);
 	}
}

void ARepelTheUprisingCharacter::DealWithHoveredSlot(URTUInventorySlot* InventorySlotIn)
{
	CurrentSlot = InventorySlotIn;
	if (CurrentSlot != nullptr)
	{
		const FName CurrentID = InventorySlotIn->GetItemID();
		UE_LOG(LogTemp, Warning, TEXT("ItemID from %s is %s"), *InventorySlotIn->GetName(), *CurrentID.ToString());

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deal with Hovered slot was called without a valid Inventory Slot"));
	}
}

void ARepelTheUprisingCharacter::AddContainerToHUD(const TSubclassOf<UUserWidget>& WidgetToCreate, URTUInventoryComponent* ContainerInventory)
{
	if (PlayerWidgetRef)
	{
		if (!bHUDIsShowing)
		{
			ChangeInputToUI();
			PlayerWidgetRef->AddContainerToHUD(WidgetToCreate, ContainerInventory, InventoryComp);
			bHUDIsShowing = true;
		}
		else
		{
			ChangeInputToGame();
			PlayerWidgetRef->RemoveCurrentWidget();
			bHUDIsShowing = false;
		}
	}
	
}
