// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "RepelTheUprisingCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ARepelTheUprisingCharacter : public ACharacter
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
	

	

	/** Test Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* TestAction;
public:
	ARepelTheUprisingCharacter();

	class URTUInventoryComponent* GetInventoryComp() const { return InventoryComp; }

protected:
	virtual void BeginPlay();

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

/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	class URTUInventoryComponent* InventoryComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	class URTUHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	class URTUStaminaComponent* StaminaComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	class URTUFoodComponent* FoodComp;

/** END OF COMPONENTS */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class URTUPlayerWidget> PlayerMenuWidget;
	
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	bool bIsCrouching;
	
	UPROPERTY()
	class ARTUPlayerState* PlayerStateRef;
	UPROPERTY()
	class ARepelTheUprisingGameMode* GameModeRef;
	void SetReferences();

	void InteractWith();

	void TogglePlayerWidget();
	UPROPERTY()
	URTUPlayerWidget* PlayerWidgetRef;
	
	FTimerHandle ReferenceDelayHandle;
};

