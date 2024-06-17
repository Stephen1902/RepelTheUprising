// Copyright Epic Games, Inc. All Rights Reserved.


#include "RepelTheUprisingPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "RepelTheUprisingCharacter.h"
#include "Engine/LocalPlayer.h"

void ARepelTheUprisingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ARepelTheUprisingPlayerController::InventorySlotHovered_Implementation(URTUInventorySlot* InventorySlotIn)
{
	if (InventorySlotIn != nullptr)
	{
		if (ARepelTheUprisingCharacter* ControlledChar = Cast<ARepelTheUprisingCharacter>(GetCharacter()))
		{
			ControlledChar->DealWithHoveredSlot(InventorySlotIn);
		}
	}

}
