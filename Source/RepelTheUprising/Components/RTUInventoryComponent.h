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

	void AddToInventory();
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
	
	float TimeSinceTraceLastCheck;
	void DoInteractTrace();

	UPROPERTY()
	AActor* CurrentlyViewedActor;
};
