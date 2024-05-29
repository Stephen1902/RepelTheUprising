// Copyright 2024 DME Games

#include "RTUInventorySlot.h"

#include "RTUActionMenuWidget.h"
#include "RTUDragDropWidget.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"

bool URTUInventorySlot::SetReferences(URTUInventoryComponent* INInventoryCompRef, FName INItemID, int32 INQuantity, int32 INContentIndex)
{
	InventoryCompRef = INInventoryCompRef;
	ItemID = INItemID;
	Quantity = INQuantity;
	ContentIndex = INContentIndex;

	return InventoryCompRef != nullptr;
}

void URTUInventorySlot::UpdateItemSlot() const
{
	if (ItemTable)
	{
		if (const FItemInformationTable* Row = ItemTable->FindRow<FItemInformationTable>(ItemID, ""))
		{
			IconImage->SetBrushFromTexture(Row->ItemThumbnail, false);
			IconImage->SetVisibility(ESlateVisibility::Visible);
			QtyTextBox->SetText(FText::FromString(FString::FromInt(Quantity)));

			if (UObject* FontObj = LoadObject<UObject>(GetWorld(), TEXT("/Script/Engine.Font'/Game/UI/Fonts/MenuFont.MenuFont'"), nullptr, LOAD_None, nullptr))
			{
				// Set the font for your UTextBlock
				FSlateFontInfo FontInfo;
				FontInfo.FontObject = FontObj;
		
				// Adjust the font size, depending on how large the quantity is
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
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Action Menu Widget tried to get MenuFont but was unable to do so."))
			}
			
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

FReply URTUInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Only do something with this if the ItemID is not set to the default;
	if (ItemID != FName("Default Name"))
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			return ReplyResult.NativeReply;			
		}
		
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			if (ActionMenuWidget)
			{
				// If an existing menu is already on screen, remove it
				if (ActionMenuWidgetRef)
				{
					ActionMenuWidgetRef->RemoveFromParent();
				}
				
				ActionMenuWidgetRef = CreateWidget<URTUActionMenuWidget>(GetWorld(), ActionMenuWidget);
				ActionMenuWidgetRef->SetReferences(InventoryCompRef, ContentIndex);
				ActionMenuWidgetRef->AddToViewport();
				return FReply::Handled();
			}
		}
	}
	return FReply::Unhandled();
}

void URTUInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	
}
