// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RepelTheUprising/Widgets/RTUInventorySlot.h"
#include "RepelTheUprisingCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
struct FInventorySlotStruct;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ARepelTheUprisingCharacter : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** 3rd person mesh, not seen by self */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh3P;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Player Menu Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PlayerMenuAction;

	/** Standard Action Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* StandardAction;

	/** Alternate Action Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AlternateAction;

	/* Standard Drag Action Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* StandardDragAction;

	/* Alternate Drag Action Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AlternateDragAction;
	
	/** Test Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* TestAction;
public:
	ARepelTheUprisingCharacter();

	class URTUInventoryComponent* GetInventoryComp() const { return InventoryComp; }

	void AddContainerToHUD(const TSubclassOf<UUserWidget>& WidgetToCreate, URTUInventoryComponent* ContainerInventory);
	void DealWithPressedButton(bool bNewPressedState);
	void ChangeInputToGame();


protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* MenuInputMapping;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* HUDInputMapping;

	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for sprinting input */
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	/** Called for crouching input */
	void ToggleCrouch(const FInputActionValue& Value);

	/** Called for test action events */
	void DoTestAction(const FInputActionValue& Value);

	/** Called for player action events */
	void StandardActionInGame(const FInputActionValue& Value);
	void AlternateActionInGame(const FInputActionValue& Value);
	void DragStandardHUD(const FInputActionValue& Value);
	void DragAlternateHUD(const FInputActionValue& Value);
	void StandardActionHUD(const FInputActionValue& Value);
	void AlternateActionHUD(const FInputActionValue& Value);

/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	URTUInventoryComponent* InventoryComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	class URTUHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	class URTUStaminaComponent* StaminaComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	class URTUFoodComponent* FoodComp;

/** END OF COMPONENTS */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class URTUPlayerHUD> PlayerHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	URTUPlayerHUD* PlayerWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class URTUDragDropWidget> DraggedWidget;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION()
	void DealWithHoveredSlot(URTUInventorySlot* InventorySlotIn);


private:
	bool bIsCrouching;
	bool bHUDIsShowing = false;
	bool bInventoryIsShowing = false;
	bool bIsDraggingLeft = false;
	bool bIsDraggingRight = false;
	bool bIsDragging = false;
	bool bInventoryButtonPressed = false;
	double TimePressed = 0.f;
	
	UPROPERTY()
	class ARTUPlayerState* PlayerStateRef;
	UPROPERTY()
	APlayerController* PlayerControllerRef;
	
	void SetReferences();

	void InteractWith();

	void TogglePlayerWidget();
	
	FTimerHandle ReferenceDelayHandle;
	void ChangeInputToUI();

	UPROPERTY()
	URTUInventorySlot* CurrentSlot;
	UPROPERTY()
	URTUDragDropWidget* DraggedSlot;
	UPROPERTY()
	FInventorySlotStruct LocalSlotStruct;
};

