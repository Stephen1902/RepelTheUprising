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
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "RepelTheUprising/Components/RTUFoodComponent.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"
#include "RepelTheUprising/Widgets/RTUInventorySlot.h"
#include "RepelTheUprising/Widgets/RTUDragDropWidget.h"
#include "RepelTheUprising/Widgets/RTUPlayerHUD.h"
#include "RepelTheUprising/Widgets/RTUPlayerStatus.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARepelTheUprisingCharacter

ARepelTheUprisingCharacter::ARepelTheUprisingCharacter()
{
	// Sets whether this actor can ever tick.  Turn it off to improve performance
	PrimaryActorTick.bCanEverTick = true;
	
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
	HotBarComp = CreateDefaultSubobject<URTUInventoryComponent>(TEXT("Hot Bar Component"));
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
		EnhancedInputComponent->BindAction(AlternateAction, ETriggerEvent::Completed, this, &ARepelTheUprisingCharacter::AlternateActionHUD);
		
		// Dragging
		EnhancedInputComponent->BindAction(StandardDragAction, ETriggerEvent::Triggered, this, &ARepelTheUprisingCharacter::DragStandardHUD);
		EnhancedInputComponent->BindAction(AlternateDragAction, ETriggerEvent::Triggered, this, &ARepelTheUprisingCharacter::DragAlternateHUD);

		// Hot Bar Actions
		EnhancedInputComponent->BindAction(HotBarKeyPress, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::HotBarKeyPressed);
		EnhancedInputComponent->BindAction(HotBarMouseScroll, ETriggerEvent::Triggered, this, &ARepelTheUprisingCharacter::HotBarMouseScrolled);
		
		// Test Action
		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Started, this, &ARepelTheUprisingCharacter::DoTestAction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARepelTheUprisingCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bInventoryButtonPressed)
	{
		TimePressed += DeltaSeconds;
		if (TimePressed >= 0.1f && !bIsDraggingRight)
		{
			bIsDraggingLeft = true;
			bIsDraggingRight = false;
			
			//DealWithPressedButton(false);
		}
	}
/*	
	if (bIsDragging)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->SetShowMouseCursor(true);
		}
	}
*/
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

	if (bHUDIsShowing || bInventoryIsShowing)
	{
		if (bIsDraggingRight)
		{
			DealWithPressedButton(false);
		}
		return;
	}
	
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
		UE_LOG(LogTemp, Warning, TEXT("Standard Action Game"));
	}
}

void ARepelTheUprisingCharacter::AlternateActionInGame(const FInputActionValue& Value)
{
	// These events only occur when the player HUD is not on screen 
	if (!bInventoryIsShowing && !bHUDIsShowing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Alternative Action Game"));
	}
}

void ARepelTheUprisingCharacter::DragStandardHUD(const FInputActionValue& Value)
{
	// Dragging can only happen when the player's HUD is on screen
	if ((bInventoryIsShowing || bHUDIsShowing) && LocalSlotStruct.InventoryCompRef != nullptr)
	{
		bIsDraggingLeft = true;
		bIsDraggingRight = false;
	}
}

void ARepelTheUprisingCharacter::DragAlternateHUD(const FInputActionValue& Value)
{
	// Dragging can only happen when the player's HUD is on screen
	if ((bInventoryIsShowing || bHUDIsShowing) && LocalSlotStruct.InventoryCompRef != nullptr)
	{
		bIsDraggingLeft = false;
		bIsDraggingRight = true;
	}
}

void ARepelTheUprisingCharacter::StandardActionHUD(const FInputActionValue& Value)
{
	// If the player is not dragging something, show the information for this item
	if (!bIsDraggingLeft && !bIsDraggingRight && !bIsDragging)
	{
		if (CurrentSlot)
		{
			
		}
	}
	else
	{
		// The player has clicked the mouse button, drop the stack
		if (CurrentSlot)
		{
		}
	}

}

void ARepelTheUprisingCharacter::AlternateActionHUD(const FInputActionValue& Value)
{
	// Alternate action in the HUD only happens when the player is dragging
	if (bIsDragging)
	{
		if ((bInventoryIsShowing || bHUDIsShowing) && CurrentSlot)
		{
			if (DraggedSlot)
			{
				if (DraggedSlot->GetQuantity() > 1)
				{
					CurrentSlot->DealWithMouseDrop(DraggedSlot->GetItemID(), 1);
					const int32 LocalQty = DraggedSlot->GetQuantity() - 1;
					DraggedSlot->SetInformation(DraggedSlot->GetItemID(), LocalQty);
				}
				else
				{
					CurrentSlot->DealWithMouseDrop(DraggedSlot->GetItemID(), DraggedSlot->GetQuantity());
					DraggedSlot->RemoveFromParent();
					DraggedSlot = nullptr;
					bIsDragging = false;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("DraggedSlot is not valid"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Is not dragging"));
	}
}

void ARepelTheUprisingCharacter::HotBarKeyPressed(const FInputActionValue& Value)
{
	if (!bInventoryIsShowing && !bHUDIsShowing && PlayerHotBarRef)
	{
		const int32 ValueAsInt = UKismetMathLibrary::Round(Value.Get<float>());
		if (ValueAsInt != CurrentHotBarValue)
		{
			CurrentHotBarValue = ValueAsInt;
			// Array of inventory grid slots starts at 0
			switch (ValueAsInt)
			{
			case 1:
				PlayerHotBarRef->HotBarValueChanged(0);
				return;
			case 2:
				PlayerHotBarRef->HotBarValueChanged(1);
				return;
			case 3:
				PlayerHotBarRef->HotBarValueChanged(2);
				return;
			case 4:
				PlayerHotBarRef->HotBarValueChanged(3);
				return;
			case 5:
				PlayerHotBarRef->HotBarValueChanged(4);
				return;
			case 6:
				PlayerHotBarRef->HotBarValueChanged(5);
				return;
			case 7:
				PlayerHotBarRef->HotBarValueChanged(6);
				return;
			case 8:
				PlayerHotBarRef->HotBarValueChanged(7);
				return;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Nothing was pressed"));
			}
		}
	}
}

void ARepelTheUprisingCharacter::HotBarMouseScrolled(const FInputActionValue& Value)
{
	// This should only happen if the player inventory is not on screen
	if (!bInventoryIsShowing && !bHUDIsShowing)
	{
		// input is a float
		const float MouseWheelVector = Value.Get<float>();

		if (MouseWheelVector != 0.f && GetHotBarComp() && PlayerHotBarRef)
		{
			if (MouseWheelVector < 0.f)
			{
				SelectedHotBarValue += 1; 
				if (SelectedHotBarValue > GetHotBarComp()->GetContents().Num() - 1)
				{
					SelectedHotBarValue = 0;
				}
			}
			else
			{
				SelectedHotBarValue -= 1;
				if (SelectedHotBarValue == -1)
				{
					SelectedHotBarValue = GetHotBarComp()->GetContents().Num() -1;	
				}
			}

			PlayerHotBarRef->HotBarValueChanged(SelectedHotBarValue);
		}
	}
}

void ARepelTheUprisingCharacter::SetReferences()
{
	PlayerStateRef = Cast<ARTUPlayerState>(GetPlayerState());
	if (!PlayerStateRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to Get Player State Ref"), *GetName());
	}

	PlayerControllerRef = Cast<APlayerController>(GetController());
	if (!PlayerControllerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to get Player Controller Ref"), *GetName());
	}

	if (PlayerHotBarWidget)
	{
		PlayerHotBarRef = CreateWidget<URTUPlayerStatus>(GetLocalViewingPlayerController(), PlayerHotBarWidget);
		PlayerHotBarRef->AddToViewport();
		PlayerHotBarRef->HotBarValueChanged(0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The player hot bar widget has not been set"));
	}
}

void ARepelTheUprisingCharacter::InteractWith()
{
	if (bHUDIsShowing)
	{
		ChangeInputToGame();
		PlayerWidgetRef->SetVisibility(ESlateVisibility::Hidden);
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
			PlayerWidgetRef->RemoveAllWidgets();
			bHUDIsShowing = false;
			ChangeInputToGame();
		}
		else
		{
			PlayerWidgetRef->AddInventoryToHUD();
			bHUDIsShowing = true;
			ChangeInputToUI();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Widget Ref is not valid"));
	}
}

void ARepelTheUprisingCharacter::ChangeInputToUI()
{
	if (PlayerControllerRef)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerRef->GetLocalPlayer());
		{
			if (MenuInputMapping && HUDInputMapping)
			{
				InputSystem->RemoveMappingContext(DefaultInputMapping);
				InputSystem->AddMappingContext(MenuInputMapping, 0);
				InputSystem->AddMappingContext(HUDInputMapping, 99);
			}
		}

		PlayerControllerRef->SetShowMouseCursor(true);
		
		FInputModeGameAndUI InputModeData;
		InputModeData.SetHideCursorDuringCapture(false);  // This line makes sure that the mouse cursor doesn't disappear on right click
		PlayerControllerRef->SetInputMode(InputModeData);

	}
}

void ARepelTheUprisingCharacter::ChangeInputToGame()
{
	// Check if there is an active widget reference and clear it if there is
	if (CurrentSlot)
	{
		CurrentSlot = nullptr;

		bIsDraggingLeft = false;
		bIsDraggingRight = false;
		bIsDragging = false;
	}
		
	if (PlayerControllerRef)
 	{
 		UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerRef->GetLocalPlayer());
 		{
 			if (HUDInputMapping && MenuInputMapping)
 			{
 				InputSystem->RemoveMappingContext(MenuInputMapping);
 				InputSystem->RemoveMappingContext(HUDInputMapping);
 				InputSystem->AddMappingContext(DefaultInputMapping, 0);
 			}
 		}
 
 		PlayerControllerRef->SetShowMouseCursor(false);
		PlayerControllerRef->SetInputMode(FInputModeGameOnly());
 	}
}

void ARepelTheUprisingCharacter::DealWithHoveredSlot(URTUInventorySlot* InventorySlotIn)
{
	CurrentSlot = InventorySlotIn;
	
	if (CurrentSlot != nullptr)
	{
		LocalSlotStruct = CurrentSlot->GetInventorySlotStruct();
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
			PlayerWidgetRef->SetVisibility(ESlateVisibility::Hidden);
			bHUDIsShowing = false;
		}
	}
}

void ARepelTheUprisingCharacter::DealWithPressedButton(const bool bNewPressedState)
{
	if (DraggedSlot != nullptr)
	{
		// The player is hovering above a stack.  Drop it.
		if (CurrentSlot)
		{
			CurrentSlot->DealWithMouseDrop(*DraggedSlot->GetItemID().ToString(), DraggedSlot->GetQuantity());
			DraggedSlot->RemoveFromParent();
			DraggedSlot = nullptr;
		}
	}
	else
	{
		bInventoryButtonPressed = bNewPressedState;
		if (!bInventoryButtonPressed)
		{
			if ((bIsDraggingLeft || bIsDraggingRight) && LocalSlotStruct.InventoryCompRef != nullptr && LocalSlotStruct.ItemID != FName(""))
			{
				bIsDragging = true;

				if (DraggedWidget)
				{
					DraggedSlot = CreateWidget<URTUDragDropWidget>(GetLocalViewingPlayerController(), DraggedWidget);
				
					if (bIsDraggingLeft || LocalSlotStruct.Quantity == 1)
					{
						LocalSlotStruct.InventoryCompRef->RemoveFromInventory(LocalSlotStruct.ContentIndex, true, false);
						DraggedSlot->SetInformation(LocalSlotStruct.ItemID, LocalSlotStruct.Quantity);
					}
					else
					{
						LocalSlotStruct.InventoryCompRef->RemoveFromInventory(LocalSlotStruct.ContentIndex, false, false);
						int32 QuantityToGet;
						if (LocalSlotStruct.Quantity % 2 == 0)
						{
							QuantityToGet = LocalSlotStruct.Quantity / 2;
						}
						else
						{
							QuantityToGet = (LocalSlotStruct.Quantity - 1) / 2;	
						}
						DraggedSlot->SetInformation(LocalSlotStruct.ItemID, QuantityToGet);
					}
				
					DraggedSlot->AddToViewport();
					CurrentSlot = nullptr;
				}
				
				bIsDraggingLeft = false;
				bIsDraggingRight = false;
			}
		}
	}
}
