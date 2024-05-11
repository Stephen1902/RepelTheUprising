// Copyright 2024 DME Games

#include "RTUInventorySlot.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"

bool URTUInventorySlot::SetReferences(URTUInventoryComponent* INInventoryCompRef, FName INItemID, int32 INQuantity)
{
	InventoryCompRef = INInventoryCompRef;
	ItemID = INItemID;
	Quantity = INQuantity;

	return InventoryCompRef != nullptr;
}

void URTUInventorySlot::UpdateItemSlot() const
{
	UE_LOG(LogTemp, Warning, TEXT("Update Item slot called"))
	if (ItemTable)
	{
		if (const FItemInformationTable* Row = ItemTable->FindRow<FItemInformationTable>(ItemID, ""))
		{
			IconImage->SetBrushFromTexture(Row->ItemThumbnail, false);
			IconImage->SetVisibility(ESlateVisibility::Visible);
			QtyTextBox->SetText(FText::FromString(FString::FromInt(Quantity)));
			// Adjust the font size, depending on how large the quantity is
			FSlateFontInfo FontInfo;
			FontInfo.Size = 8;
			if (Quantity > 9 && Quantity < 100)
			{
				FontInfo.Size = 7;
			}
			if (Quantity > 99 && Quantity < 1000)
			{
				FontInfo.Size = 6;
			}
			if (Quantity > 999)
			{
				FontInfo.Size = 4.5;
			}
		
			QtyTextBox->SetFont(FontInfo);			
			QtySizeBox->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			IconImage->SetVisibility(ESlateVisibility::Hidden);
			QtySizeBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void URTUInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	
}
