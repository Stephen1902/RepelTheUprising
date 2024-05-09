// Copyright 2024 DME Games


#include "RTUInventoryComponent.h"
#include "RTUItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"

#define InteractTrace ECC_GameTraceChannel2

// Sets default values for this component's properties
URTUInventoryComponent::URTUInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableInfo(TEXT("'/Game/Items/DT_ItemInfo'"));
	if (DataTableInfo.Succeeded())
	{
		ItemTable = DataTableInfo.Object;
	}

	TimeBetweenTraceCheck = 0.1f;
	TimeSinceTraceLastCheck = 0.f;
	InteractionRange = 300.f;
	
	SetIsReplicatedByDefault(true);
}

void URTUInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Check whether what this component is connected to is a player
	PlayerCharacterRef = Cast<ARepelTheUprisingCharacter>(GetOwner());

	SlotStruct.SetNum(InventorySlots);
}

void URTUInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Only the player character needs to do a line trace
	if (PlayerCharacterRef != nullptr)
	{
		TimeSinceTraceLastCheck += DeltaTime;
		if (TimeSinceTraceLastCheck >= TimeBetweenTraceCheck)
		{
			DoInteractTrace();
		}
	}
}

bool URTUInventoryComponent::AddToInventory(FName ItemID, int32 Quantity, int32& QuantityRemaining)
{
	// Used to keep track of how many are left to try to add to the inventory
	int32 LocalQty = Quantity;

	// Used to check there is not an infinite loop
	bool bHasFailed = false;

	while (LocalQty > 0 && !bHasFailed)
	{
		int32 FoundSlot = FindExistingSlot(ItemID);
		if (FoundSlot >= 0)
		{
			// Found a slot with space in the array, add to this stack
			const int32 AvailableSpaces = GetMaxStackSize(ItemID) - SlotStruct[FoundSlot].Quantity;
			if (AvailableSpaces >= Quantity)
			{
				// There is space for the entire stack, add it all	
				AddToStack(FoundSlot, Quantity);
				LocalQty = 0;
			}
			else
			{
				// Only part of the stack can be added
				AddToStack(FoundSlot, AvailableSpaces);
				LocalQty -= AvailableSpaces;
			}
		}
		else
		{
			// There is no existing slot with space, check for an empty slot
			FoundSlot = HasEmptySlot(); 
			if (FoundSlot >= 0)
			{
				SlotStruct[FoundSlot].ItemID = ItemID;
				SlotStruct[FoundSlot].Quantity = LocalQty;
				LocalQty = 0;
			}
			else
			{
				// There is no existing slots and no space available.
				bHasFailed = true;
			}
		}
	}

	QuantityRemaining = LocalQty;
	return !bHasFailed;
}

void URTUInventoryComponent::RemoveFromInventory()
{
	
}

int32 URTUInventoryComponent::FindExistingSlot(FName ItemID)
{
	for (int32 i = 0; i < SlotStruct.Num(); ++i)
	{
		if (SlotStruct[i].ItemID == ItemID)
		{
			if (SlotStruct[i].Quantity < GetMaxStackSize(ItemID))
			{
				return i;
			}

		}
	}
	
	return -1;
}

int32 URTUInventoryComponent::HasEmptySlot()
{
	for (int32 i = 0; i < SlotStruct.Num(); ++i)
	{
		if (SlotStruct[i].Quantity == 0)
		{
			return i;
		}
	}
	
	return -1;
}

int32 URTUInventoryComponent::GetMaxStackSize(FName ItemID) const
{
	if (ItemTable)
	{
		if (const FItemInformationTable* Row = ItemTable->FindRow<FItemInformationTable>(ItemID, ""))
		{
			return Row->MaxStackSize;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid table found in Inventory Component of %s"), *GetName());
	}

	return -1;

}

void URTUInventoryComponent::AddToStack(int32 Index, int32 QuantityToAdd)
{
	if (QuantityToAdd > 0)
	{
		SlotStruct[Index].Quantity += QuantityToAdd;
	}
}

void URTUInventoryComponent::CreateNewStack(int32 FoundSlot, FName ItemID, int32 QuantityToAdd)
{

}

void URTUInventoryComponent::InteractWithItem()
{
	// This only needs to be done if there is a valid actor to interact with
	if (CurrentlyViewedActor)
	{
		Server_InteractWithItem(CurrentlyViewedActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No CurrentlyViewedActor"));
	}
}

void URTUInventoryComponent::Server_InteractWithItem_Implementation(AActor* TargetActor)
{
	UActorComponent* ItemComponent = CurrentlyViewedActor->GetComponentByClass(URTUItemComponent::StaticClass());
	if (ItemComponent)
	{
		Execute_InteractWith(ItemComponent, PlayerCharacterRef);
		DEBUG();
	}
	else  // What to do with items that don't have an item component ie a chest
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid Item Component"));	
	}
}

bool URTUInventoryComponent::Server_InteractWithItem_Validate(AActor* TargetActor)
{
	return PlayerCharacterRef != nullptr;
}

void URTUInventoryComponent::DoInteractTrace()
{
	TimeSinceTraceLastCheck = 0.f;

	FVector EyesLoc;
	FRotator EyesRot;
	PlayerCharacterRef->GetActorEyesViewPoint(EyesLoc, EyesRot);

	const FVector EndLoc = (EyesRot.Vector() * InteractionRange) + EyesLoc;
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, EyesLoc, EndLoc, InteractTrace, QueryParams);

	if (HitResult.IsValidBlockingHit())
	{
		// Check that the actor we hit is a new one
		if (CurrentlyViewedActor != HitResult.GetActor())
		{
			CurrentlyViewedActor = HitResult.GetActor();
			const FText TextToDisplay = Execute_LookAt(CurrentlyViewedActor);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *TextToDisplay.ToString());
		}
	}
	else
	{
		if (CurrentlyViewedActor != nullptr)
		{
			// The player isn't looking at an interactive item, clear it
			CurrentlyViewedActor = nullptr;
		}
	}
}

void URTUInventoryComponent::DEBUG()
{
	for (int32 i = 0; i < SlotStruct.Num(); ++i)
	{
		const FString StringToDisplay = SlotStruct[i].ItemID.ToString() + " : " + FString::FromInt(SlotStruct[i].Quantity);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *StringToDisplay);
	}
}


void URTUInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUInventoryComponent, SlotStruct);
}

