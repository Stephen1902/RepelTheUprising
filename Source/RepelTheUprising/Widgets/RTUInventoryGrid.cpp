// Copyright 2024 DME Games


#include "RTUInventoryGrid.h"
#include "RTUInventorySlot.h"
#include "Components/WrapBox.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"

void URTUInventoryGrid::SetReferences(URTUInventoryComponent* InventoryComponentIN)
{
	if (InventoryComponentIN != nullptr)
	{
		InventoryCompRef = InventoryComponentIN;
		PopulateWrapBox();
	}
}

void URTUInventoryGrid::PopulateWrapBox()
{
	// Remove all existing items already created
	GridWrapBox->ClearChildren();

	TArray<FSlotStruct> InventoryContents = InventoryCompRef->GetContents();

	if (InventoryContents.Num() > 0  && SlotWidget)
	{
		for (int32 i = 0; i < InventoryContents.Num(); ++i)
		{
			if (URTUInventorySlot* CurrentSlot = CreateWidget<URTUInventorySlot>(GetOwningPlayer(), SlotWidget))
			{
					if (bool bSuccess = CurrentSlot->SetReferences(InventoryCompRef, InventoryContents[i].ItemID, InventoryContents[i].Quantity, i))
					{
						CurrentSlot->UpdateItemSlot();
						GridWrapBox->AddChildToWrapBox(CurrentSlot);
					}
			}
		}
	}
}

