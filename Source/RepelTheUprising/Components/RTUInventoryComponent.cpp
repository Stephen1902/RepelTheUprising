// Copyright 2024 DME Games


#include "RTUInventoryComponent.h"

#include "RTUFoodComponent.h"
#include "RTUHealthComponent.h"
#include "RTUItemComponent.h"
#include "RTUStaminaComponent.h"
#include "RTUWaterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"
#include "RepelTheUprising/Items/GameItemBase.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"
#include "RepelTheUprising/Widgets/RTUInventorySlot.h"
#include "RepelTheUprising/Widgets/RTU_DisplayMessage.h"

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

int32 URTUInventoryComponent::GetHalfOfQuantity(const int32 QuantityIn, const int32 IndexToChange )
{
	// Check if the quantity is divisible by 2
	if (QuantityIn % 2 == 0)
	{
		SlotStruct[IndexToChange].Quantity = QuantityIn / 2;
		UpdateInventory();
		return QuantityIn / 2;
	}

	// Leave an extra one in the inventory when taking "half"
	SlotStruct[IndexToChange].Quantity = (QuantityIn + 1) / 2;
	UpdateInventory();
	return (QuantityIn - 1) / 2;
}

void URTUInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Check whether what this component is connected to is a player
	PlayerCharacterRef = Cast<ARepelTheUprisingCharacter>(GetOwner());

	SlotStruct.SetNum(InventorySlots);

	AddMessageWidget();
}

void URTUInventoryComponent::Server_DropItem_Implementation(FName ItemID, int32 Quantity)
{
	if (FItemInformationTable* Row = GetCurrentItemInfo(ItemID))
	{
		FTransform NewTransform;
		NewTransform.SetLocation(GetDropLocation());
		NewTransform.SetRotation(FQuat(0.f, 0.f, UKismetMathLibrary::RandomIntegerInRange(0, 359), 0.f));
		NewTransform.SetScale3D(FVector(1.f));
		if (Quantity == 1)
		{
			GetWorld()->SpawnActor<AActor>(Row->ActorOnDrop, NewTransform, FActorSpawnParameters());
		}
		else
		{
			SpawnLootBag(ItemID, Quantity);
		}
	}
}

void URTUInventoryComponent::Server_RemoveFromInventory_Implementation(int32 ItemIndexIN, bool RemoveWholeStackIN, bool ConsumeIN)
{
	RemoveFromInventory(ItemIndexIN, RemoveWholeStackIN, ConsumeIN);
}

void URTUInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

bool URTUInventoryComponent::QueryInventory(FName INItemID, int32 QuantityRequired, int32& QuantityFound)
{
	if (INItemID != FName("Default Name"))
	{
		int32 LocalQtyFound = 0;

		for (auto& It : SlotStruct)
		{
			if (It.ItemID == INItemID)
			{
				LocalQtyFound += It.Quantity;
			}
		}

		QuantityFound = LocalQtyFound;
		if (QuantityFound >= QuantityRequired)
		{
			return true;
		}
	}
	
	return false;
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
				if (GetMaxStackSize(ItemID) < LocalQty)
				{
					SlotStruct[FoundSlot].Quantity = GetMaxStackSize(ItemID);
					LocalQty -= GetMaxStackSize(ItemID);
				}
				else
				{
					SlotStruct[FoundSlot].Quantity = LocalQty;
					LocalQty = 0;
				}
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

void URTUInventoryComponent::RemoveFromInventory(int32 ItemIndex, bool RemoveWholeStack, bool Consume)
{
	FName LocalItemID = SlotStruct[ItemIndex].ItemID;
	int32 LocalQty = SlotStruct[ItemIndex].Quantity;

	if (RemoveWholeStack || LocalQty == 1)
	{
		SlotStruct[ItemIndex].ItemID = FName("");
		SlotStruct[ItemIndex].Quantity = 0;

		if (Consume && PlayerCharacterRef)
		{
			Server_ConsumeItem(LocalItemID);
		}
		else
		{
			Server_DropItem(LocalItemID, LocalQty);
		}
	}
	else
	{
		// We are only removing one from the stack
		LocalQty -= 1;
		SlotStruct[ItemIndex].Quantity = LocalQty;

		// Check if one is being removed because we are consuming this
		if (Consume && PlayerCharacterRef)
		{
			Server_ConsumeItem(LocalItemID);
		}
		else
		{
			// Otherwise, the item is to be dropped into the world
			Server_DropItem(LocalItemID, 1);	
		}
	}

	UpdateInventory();
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

int32 URTUInventoryComponent::GetMaxStackSize(FName ItemID)
{
	if (const FItemInformationTable* Row = GetCurrentItemInfo(ItemID))
	{
		return Row->MaxStackSize;
	}
	
	// Failed to get a valid row
	UE_LOG(LogTemp, Warning, TEXT("Failed to get a valid Row in %s"), *GetName());
	return -1;
}

void URTUInventoryComponent::AddToStack(int32 Index, int32 QuantityToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Adding To Stack"))
	if (QuantityToAdd > 0)
	{
		SlotStruct[Index].Quantity += QuantityToAdd;
		UpdateInventory();
	}
}

void URTUInventoryComponent::CreateNewStack(int32 FoundSlot, FName ItemID, int32 QuantityToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("FoundSlot: %i, ItemID: %s, QtyToAdd: %i"), FoundSlot, *ItemID.ToString(), QuantityToAdd);
	if (QuantityToAdd > 0)
	{
		SlotStruct[FoundSlot].ItemID = ItemID;
		SlotStruct[FoundSlot].Quantity = QuantityToAdd;
		UpdateInventory();
	}
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

bool URTUInventoryComponent::TransferSlots(int32 SourceIndex, URTUInventoryComponent* SourceInventory, int32 TargetIndex)
{
	if (SourceInventory == nullptr)
	{
		return false;
	}
	
	TArray<FSlotStruct> LocalSource = SourceInventory->GetContents();
	const FSlotStruct SourceToUse = LocalSource[SourceIndex];

	if (TargetIndex < 0)
	{
	
	}
	else
	{
		// Check if what is being dropped is the same as what is already in the array
		if (SourceToUse.ItemID == SlotStruct[TargetIndex].ItemID)
		{
			// Get the total amount of items from both inventories to check against the max stack size
			const int32 TotalSlotQty = SlotStruct[TargetIndex].Quantity + SourceToUse.Quantity;
			// Check against the max stack size to see if there are any left over
			
			if (const int32 RemainingQty = TotalSlotQty - GetMaxStackSize(SlotStruct[TargetIndex].ItemID) > 0)
			{
				// There is a value left over.  Put it back in the source inventory
				//SourceInventory->SlotStruct[SourceIndex].ItemID = SlotStruct[TargetIndex].ItemID;
				SourceInventory->SlotStruct[SourceIndex].Quantity = RemainingQty;
				SourceInventory->UpdateInventory();
				
				// Set the target inventory to the max stack size 
				SlotStruct[TargetIndex].Quantity = GetMaxStackSize(SlotStruct[TargetIndex].ItemID);
			}
			else
			{
				// The amount does not exceed the max stack size, set the target to the total
				SlotStruct[TargetIndex].Quantity = TotalSlotQty;
			}
			
			UpdateInventory();
		}
		else
		{
			SourceInventory->SwapElements(SlotStruct[TargetIndex], SourceIndex);
			SlotStruct[TargetIndex] = SourceToUse;
			// Update the inventory on both client and server, both local and source inventories
			UpdateInventory();
			if (SourceInventory != this)
			{
				SourceInventory->UpdateInventory();
			}
		}
	}
	
	return true;
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
		if (TargetActor)
		{
			const ARepelTheUprisingCharacter* OwningPlayer = Cast<ARepelTheUprisingCharacter>(GetOwner());
			AActor* OwningController = (OwningPlayer->GetController());
			TargetActor->SetOwner(OwningController);
			Client_Interact(TargetActor, GetOwner());
		}
	}
}

bool URTUInventoryComponent::Server_InteractWithItem_Validate(AActor* TargetActor)
{
	return PlayerCharacterRef != nullptr;
}

void URTUInventoryComponent::Client_Interact_Implementation(AActor* ActorToInteractWith, AActor* InteractingActor)
{
	Execute_InteractWith(ActorToInteractWith, Cast<ARepelTheUprisingCharacter>(InteractingActor));
}

bool URTUInventoryComponent::Client_Interact_Validate(AActor* ActorToInteractWith, AActor* InteractingActor)
{
	return InteractingActor != nullptr;
}


void URTUInventoryComponent::UpdateInventory_Implementation()
{
	OnInventoryUpdated.Broadcast();
}

void URTUInventoryComponent::Server_TransferSlots_Implementation(const int32 SourceIndex, URTUInventoryComponent* SourceInventory, const int32 TargetIndex)
{
	TransferSlots(SourceIndex, SourceInventory, TargetIndex);
}

bool URTUInventoryComponent::Server_TransferSlots_Validate(int32 SourceIndex,	URTUInventoryComponent* SourceInventory, int32 TargetIndex)
{
	return SourceInventory != nullptr;
}

void URTUInventoryComponent::AddMessageWidget()
{
	// Only the player needs to have a widget to display on screen
	if (PlayerCharacterRef && DisplayMessageWidget)
	{
		APlayerController* OwningController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (OwningController)
		{
			DisplayMessageRef = CreateWidget<URTU_DisplayMessage>(OwningController, DisplayMessageWidget);
			if (DisplayMessageRef)
			{
				DisplayMessageRef->AddToViewport();
			}
		}
	}
}

void URTUInventoryComponent::ConsumeItem(const int32 ItemIndex)
{
	FName LocalItemID = SlotStruct[ItemIndex].ItemID;
	int32 LocalQty = SlotStruct[ItemIndex].Quantity;

	Server_RemoveFromInventory(ItemIndex, false, true);
}

void URTUInventoryComponent::Server_ConsumeItem_Implementation(FName INItemID)
{
	if (const FItemInformationTable* ItemInfo = GetCurrentItemInfo(INItemID))
	{
		if (ItemInfo->OnConsumeStruct.EffectOnFood != 0.f)
		{
			if (URTUFoodComponent* FoodComp = Cast<URTUFoodComponent>( PlayerCharacterRef->GetComponentByClass(URTUFoodComponent::StaticClass())))
			{
				FoodComp->ConsumeFood(ItemInfo->OnConsumeStruct.EffectOnFood);
			}
		}
		
		if (ItemInfo->OnConsumeStruct.EffectOnThirst != 0.f)
		{
			if (URTUWaterComponent* WaterComp = Cast<URTUWaterComponent>( PlayerCharacterRef->GetComponentByClass(URTUWaterComponent::StaticClass())))
			{
				WaterComp->ConsumeWater(ItemInfo->OnConsumeStruct.EffectOnThirst);
			}
		}

		if (ItemInfo->OnConsumeStruct.EffectOnStamina != 0.f)
		{
			if (URTUStaminaComponent* StaminaComp = Cast<URTUStaminaComponent>(PlayerCharacterRef->GetComponentByClass(URTUStaminaComponent::StaticClass())))
			{
				StaminaComp->AdjustStamina(ItemInfo->OnConsumeStruct.EffectOnStamina);
			}
		}

		if (ItemInfo->OnConsumeStruct.EffectOnHealth != 0.f)
		{
			if (URTUHealthComponent* HealthComp = Cast<URTUHealthComponent>(PlayerCharacterRef->GetComponentByClass(URTUHealthComponent::StaticClass())))
			{
				HealthComp->Heal(ItemInfo->OnConsumeStruct.EffectOnHealth);
			}
		}
	}	
}

void URTUInventoryComponent::SpawnLootBag(const FName INItemID, const int32 INQuantity)
{
	if (FItemInformationTable* Row = GetCurrentItemInfo("lootbag"))
	{
		FTransform NewTransform;
		NewTransform.SetLocation(GetDropLocation());
		NewTransform.SetRotation(FQuat(0.f, 0.f, UKismetMathLibrary::RandomIntegerInRange(0, 359), 0.f));
		NewTransform.SetScale3D(FVector(1.f));
		
		AGameItemBase* NewBase = GetWorld()->SpawnActor<AGameItemBase>(Row->ActorOnDrop, NewTransform, FActorSpawnParameters());
		NewBase->SetItemInfo(INItemID, INQuantity);
	}
}

FName URTUInventoryComponent::GetIDAtIndex(const int32 IndexToCheck)
{
	if (IndexToCheck > -1)
	{
		return SlotStruct[IndexToCheck].ItemID;
	}

	return FName("");
}

FItemInformationTable* URTUInventoryComponent::GetCurrentItemInfo(FName INItemID)
{
	if (ItemTable)
	{
		if (FItemInformationTable* Row = ItemTable->FindRow<FItemInformationTable>(INItemID, ""))
		{
			return Row;
		}
	}

	return nullptr;
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
			if (PlayerCharacterRef && PlayerCharacterRef->IsLocallyControlled() && DisplayMessageRef)
			{
				const FText TextToDisplay = Execute_LookAt(CurrentlyViewedActor);
				DisplayMessageRef->SetMessageText(TextToDisplay);
			}
		}
	}
	else
	{
		if (CurrentlyViewedActor != nullptr)
		{
			// The player isn't looking at an interactive item, clear it
			CurrentlyViewedActor = nullptr;
			if (DisplayMessageRef)
			{
				DisplayMessageRef->SetMessageText(FText::FromString(""));
			}
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

void URTUInventoryComponent::SwapElements(const FSlotStruct INItemID, int32 INSlotToUse)
{
	SlotStruct[INSlotToUse] = INItemID;
}

FVector URTUInventoryComponent::GetDropLocation() const
{
	const FVector StartLoc = (UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetOwner()->GetActorForwardVector(), 30) * 150) + GetOwner()->GetActorLocation();
	const FVector EndLoc = StartLoc - FVector(0.f, 0.f, 500.f);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility))
	{
		return HitResult.Location;
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed to get a valid drop location"));
	return FVector(0.f);
}

void URTUInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUInventoryComponent, SlotStruct);
}

