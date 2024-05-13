// Copyright 2024 DME Games


#include "RTUDragDropWidget.h"
#include "Components/Image.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"

void URTUDragDropWidget::SetItemID(const FName INItemID)
{
	ItemID = INItemID;

	if (ItemInfoTable)
	{
		if (const FItemInformationTable* Row = ItemInfoTable->FindRow<FItemInformationTable>(ItemID, ""))
		{
			DDImage->SetBrushFromTexture(Row->ItemThumbnail, false);
		}
	}
}

void URTUDragDropWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemInfoTable)
	{
		if (const FItemInformationTable* Row = ItemInfoTable->FindRow<FItemInformationTable>(ItemID, ""))
		{
			DDImage->SetBrushFromTexture(Row->ItemThumbnail, false);
		}
	}
}
