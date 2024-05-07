// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
class REPELTHEUPRISING_API URTUInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTUInventoryComponent();

	void AddToInventory();
	void RemoveFromInventory();
	
protected:
	// Number available inventory slots this component has 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Inventory")
	TArray<FSlotStruct> SlotStruct;
};
