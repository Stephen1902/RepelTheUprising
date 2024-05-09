// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RTUInventoryComponent.generated.h"

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

	bool AddToInventory(FName ItemID, int32 Quantity, int32& QuantityRemaining);
	void RemoveFromInventory();
	void InteractWithItem();

	AActor* GetCurrentActor() const { return CurrentlyViewedActor; }
	
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

	UFUNCTION(Server, WithValidation, Reliable, Category = "Functions")
	void Server_InteractWithItem(AActor* TargetActor);

private:
	UPROPERTY()
	class ARepelTheUprisingCharacter* PlayerCharacterRef;

	// Data table where the item information is stored
	UPROPERTY()
	UDataTable* ItemTable;	

	float TimeSinceTraceLastCheck;
	void DoInteractTrace();

	UPROPERTY()
	AActor* CurrentlyViewedActor;

	// Used to find a slot with the same item or an empty slot in the array
	int32 FindExistingSlot(FName ItemID);
	int32 HasEmptySlot();

	int32 GetMaxStackSize(FName ItemID) const;
	void CreateNewStack(int32 ItemIndex, FName ItemID, int32 QuantityToAdd);
	void AddToStack(int32 IndexToAddTo, int32 QuantityToAdd);

	//  DEBUG ONLY - REMOVE
	void DEBUG();
};