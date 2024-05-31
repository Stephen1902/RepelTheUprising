// Copyright 2024 DME Games


#include "RTUActionMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"

void URTUActionMenuWidget::SetReferences(URTUInventoryComponent* InventoryComponentIN, int32 IndexIN)
{
	InventoryComponentRef = InventoryComponentIN;
	ContentIndex = IndexIN;
}

void URTUActionMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	FButtonStyle ButtonStyle;
	ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.0f));
	ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	
	MenuButtonUse->SetStyle(ButtonStyle);
	MenuButtonUse->OnClicked.AddDynamic(this, &URTUActionMenuWidget::UseButtonClicked);
	MenuButtonDropOne->SetStyle(ButtonStyle);
	MenuButtonDropOne->OnClicked.AddDynamic(this, &URTUActionMenuWidget::DropOneButtonClicked);
	MenuButtonDropAll->SetStyle(ButtonStyle);
	MenuButtonDropAll->OnClicked.AddDynamic(this, &URTUActionMenuWidget::DropAllButtonClicked);
	
	MenuTextUse->SetText(FText::FromString("Use"));
	MenuTextDropOne->SetText(FText::FromString("Drop One"));
	MenuTextDropAll->SetText(FText::FromString("Drop All"));

	if (UObject* FontObj = LoadObject<UObject>(this, TEXT("/Script/Engine.Font'/Game/UI/Fonts/MenuFont.MenuFont'"), nullptr, LOAD_None, nullptr))
	{
		// Set the font for your UTextBlock
		FSlateFontInfo FontInfo;
		FontInfo.FontObject = FontObj;
		FontInfo.Size = 14;
		
		MenuTextUse->SetFont(FontInfo);
		MenuTextDropOne->SetFont(FontInfo);
		MenuTextDropAll->SetFont(FontInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Action Menu Widget tried to get MenuFont but was unable to do so."))
	}
}

void URTUActionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FVector2d MousePos = UWidgetLayoutLibrary::GetMousePositionOnPlatform();
	UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(MenuVerticalBox);
	PanelSlot->SetPosition(MousePos);
}

void URTUActionMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	RemoveFromParent();	
}

void URTUActionMenuWidget::UseButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("UseButtonClicked"));
	if (InventoryComponentRef)
	{
		InventoryComponentRef->Server_RemoveFromInventory(ContentIndex, false, true);
	}
}

void URTUActionMenuWidget::DropOneButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("DropOneButtonClicked"));
	if (InventoryComponentRef)
	{
		InventoryComponentRef->Server_RemoveFromInventory(ContentIndex, false, false);
	}
}

void URTUActionMenuWidget::DropAllButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("DropAllButtonClicked"));
	if (InventoryComponentRef)
	{
		InventoryComponentRef->Server_RemoveFromInventory(ContentIndex, true, false);
	}
}
