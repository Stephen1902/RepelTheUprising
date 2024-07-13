// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RepelTheUprisingPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class REPELTHEUPRISING_API ARepelTheUprisingPlayerController : public APlayerController, public IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ClearPlayerHUD();
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
	
	virtual void BeginPlay() override;

	// Begin Actor interface
	virtual void InventorySlotHovered_Implementation(class URTUInventorySlot* InventorySlotIn) override;
	virtual void InventoryButtonPressed_Implementation(bool NewPressedState) override;
	// End Actor interface
};
