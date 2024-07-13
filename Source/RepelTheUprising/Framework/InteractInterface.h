// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REPELTHEUPRISING_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactive")
	FText LookAt();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactive")
	void InteractWith(class ARepelTheUprisingCharacter* CharacterWhoInteracted);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactive")
	void InventorySlotHovered(class URTUInventorySlot* SlotBeingHovered);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactive")
	void InventoryButtonPressed(bool NewPressedState);
};
