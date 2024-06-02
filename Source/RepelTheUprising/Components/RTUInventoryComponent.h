// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"
#include "RTUInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Struct")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Struct")
	int32 Quantity;

	FSlotStruct()
	{
		ItemID = TEXT("Default Name");
		Quantity = 0;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUInventoryComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTUInventoryComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool QueryInventory(FName INItemID, int32 QuantityRequired, int32& QuantityFound);
	bool AddToInventory(FName ItemID, int32 Quantity, int32& QuantityRemaining);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveFromInventory(int32 ItemIndex, bool RemoveWholeStack, bool Consume);
	void InteractWithItem();
	bool TransferSlots(int32 SourceIndex, URTUInventoryComponent* SourceInventory, int32 TargetIndex);

	AActor* GetCurrentActor() const { return CurrentlyViewedActor; }
	TArray<FSlotStruct> GetContents() const { return SlotStruct; }

	UPROPERTY()
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(Server, Unreliable, Category="Functions")
	void Server_RemoveFromInventory(int32 ItemIndexIN, bool RemoveWholeStackIN, bool ConsumeIN);

protected:
	// Called on game start
	virtual void BeginPlay() override;
	
	// Number available inventory slots this component has 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Inventory")
	TArray<FSlotStruct> SlotStruct;

	// How far in front of the player can they interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Inventory")
	double InteractionRange;
	
	// How often to do a line trace in seconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Inventory")
	double TimeBetweenTraceCheck;

	// Display message widget to show on screen
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Inventory")
	TSubclassOf<class URTU_DisplayMessage> DisplayMessageWidget;

	UFUNCTION(Server, WithValidation, Reliable, Category = "Functions")
	void Server_InteractWithItem(AActor* TargetActor);
	
	UFUNCTION(Server, Unreliable, Category="Functions")
	void Server_DropItem(FName ItemID, int32 Quantity);

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Functions")
	void Server_TransferSlots(int32 SourceIndex, URTUInventoryComponent* SourceInventory, int32 TargetIndex);

	UFUNCTION(Server, Unreliable, Category="Functions")
	void Server_ConsumeItem(FName INItemID);
	
	UFUNCTION(NetMulticast, reliable)
	void UpdateInventory();

	UFUNCTION(Client, WithValidation, Reliable, Category = "Functions")
	void Client_Interact(AActor* ActorToInteractWith, AActor* InteractingActor);

private:
	UPROPERTY()
	ARepelTheUprisingCharacter* PlayerCharacterRef;

	// Data table where the item information is stored
	UPROPERTY()
	UDataTable* ItemTable;

	FItemInformationTable* GetCurrentItemInfo(FName INItemID);

	float TimeSinceTraceLastCheck;
	void DoInteractTrace();

	UPROPERTY()
	AActor* CurrentlyViewedActor;

	// Used to find a slot with the same item or an empty slot in the array
	int32 FindExistingSlot(FName ItemID);
	int32 HasEmptySlot();

	int32 GetMaxStackSize(FName ItemID);
	void CreateNewStack(int32 ItemIndex, FName ItemID, int32 QuantityToAdd);
	void AddToStack(int32 IndexToAddTo, int32 QuantityToAdd);

	//  DEBUG ONLY - REMOVE
	void DEBUG();

	void SwapElements(const FSlotStruct INItemID, int32 INSlotToUse);

	FVector GetDropLocation() const;

	UPROPERTY()
	TObjectPtr<URTU_DisplayMessage> DisplayMessageRef;

	void AddMessageWidget();
	void ConsumeItem(const int32 ItemIndex);
};