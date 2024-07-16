// Copyright 2024 DME Games

#include "RTUInventorySlot.h"
#include "RTUActionMenuWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"
#include "RepelTheUprising/Player/RepelTheUprisingPlayerController.h"

bool URTUInventorySlot::SetReferences(URTUInventoryComponent* INInventoryCompRef, FName INItemID, int32 INQuantity, int32 INContentIndex)
{
	InventorySlotStruct.InventoryCompRef = INInventoryCompRef;
	InventorySlotStruct.ItemID = INItemID;
	InventorySlotStruct.Quantity = INQuantity;
	InventorySlotStruct.ContentIndex = INContentIndex;

	return InventorySlotStruct.InventoryCompRef != nullptr;
}

void URTUInventorySlot::UpdateItemSlot() const
{
	if (ItemTable)
	{
		if (const FItemInformationTable* Row = ItemTable->FindRow<FItemInformationTable>(InventorySlotStruct.ItemID, ""))
		{
			IconImage->SetBrushFromTexture(Row->ItemThumbnail, false);
			IconImage->SetVisibility(ESlateVisibility::Visible);
			QtyTextBox->SetText(FText::FromString(FString::FromInt(InventorySlotStruct.Quantity)));

			if (UObject* FontObj = LoadObject<UObject>(GetWorld(), TEXT("/Script/Engine.Font'/Game/UI/Fonts/MenuFont.MenuFont'"), nullptr, LOAD_None, nullptr))
			{
				// Set the font for your UTextBlock
				FSlateFontInfo FontInfo;
				FontInfo.FontObject = FontObj;
		
				// Adjust the font size, depending on how large the quantity is
				FontInfo.Size = 8;
			
				if (InventorySlotStruct.Quantity > 9 && InventorySlotStruct.Quantity < 100)
				{
					FontInfo.Size = 7;
				}
				if (InventorySlotStruct.Quantity > 99 && InventorySlotStruct.Quantity < 1000)
				{
					FontInfo.Size = 6;
				}
				if (InventorySlotStruct.Quantity > 999)
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

void URTUInventorySlot::GetSlotParent()
{
	UE_LOG(LogTemp, Warning, TEXT("Parent is %s"), *GetParent()->GetName());
}

void URTUInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	HighlightColourInventory = FLinearColor(1.0f, 1.0f, 0.2f);
	HighlightColourHotBar = FLinearColor(.01f, 0.36f, .09f);
	
}

void URTUInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	SlotButton->OnHovered.AddDynamic(this, &URTUInventorySlot::OnButtonHovered);
	SlotButton->OnUnhovered.AddDynamic(this, &URTUInventorySlot::OnButtonUnhovered);
	SlotButton->OnPressed.AddDynamic(this, &URTUInventorySlot::OnButtonPressed);
	SlotButton->OnReleased.AddDynamic(this, &URTUInventorySlot::OnButtonReleased);

	DefaultHighlightColour = OuterBorder->GetBrushColor();
	
}

void URTUInventorySlot::DealWithMouseDrop(FName InItemID, int32 InQuantity)
{
	if (InventorySlotStruct.InventoryCompRef)
	{
		if (InventorySlotStruct.InventoryCompRef->GetIDAtIndex(InventorySlotStruct.ContentIndex) == InItemID)
		{
			InventorySlotStruct.InventoryCompRef->AddToStack(InventorySlotStruct.ContentIndex, InQuantity);
		}
		else
		{
			InventorySlotStruct.InventoryCompRef->CreateNewStack(InventorySlotStruct.ContentIndex, InItemID, InQuantity);
		}
	}
}
/*
FReply URTUInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Only do something with this if the ItemID is not set to the default;
/*	if (ItemID != FName("Default Name"))
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			bIsRightMouseButton = false;
			FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			return ReplyResult.NativeReply;			
		}
		
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && Quantity > 0)
		{
			bIsRightMouseButton = true;
			FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton);
			return ReplyResult.NativeReply;	
			if (ActionMenuWidget)
			{
				// If an existing menu is already on screen, remove it
				if (ActionMenuWidgetRef)
				{
					ActionMenuWidgetRef->RemoveFromParent();
					ActionMenuWidgetRef = nullptr;
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
*/
void URTUInventorySlot::OnButtonHovered()
{
	Execute_InventorySlotHovered(Cast<ARepelTheUprisingPlayerController>(GetOwningPlayer()), this);
	OuterBorder->SetBrushColor(HighlightColourInventory);
//	FSlateBrush NewBrush;
//	NewBrush.TintColor = FSlateColor(HighlightColour);
//	OuterBorder->SetBrush(NewBrush);
}

void URTUInventorySlot::OnButtonUnhovered()
{
	//Execute_InventorySlotHovered(Cast<ARepelTheUprisingPlayerController>(GetOwningPlayer()), nullptr);
	OuterBorder->SetBrushColor(DefaultHighlightColour);
//	FSlateBrush NewBrush;
//	NewBrush.TintColor = FSlateColor(HighlightColour);
//	OuterBorder->SetBrush(NewBrush);
}

void URTUInventorySlot::OnButtonPressed()
{
	Execute_InventoryButtonPressed(Cast<ARepelTheUprisingPlayerController>(GetOwningPlayer()), true);
}

void URTUInventorySlot::OnButtonReleased()
{
	Execute_InventoryButtonPressed(Cast<ARepelTheUprisingPlayerController>(GetOwningPlayer()), false);
}
