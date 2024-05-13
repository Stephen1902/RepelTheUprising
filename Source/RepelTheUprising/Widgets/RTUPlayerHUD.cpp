// Copyright 2024 DME Games


#include "RTUPlayerHUD.h"
#include "RTUInventoryWidget.h"

void URTUPlayerHUD::AddInventoryToHUD()
{
	if (InventoryWidgetRef)
	{
		CurrentWidgetRef = CreateWidget(GetOwningPlayer(), InventoryWidgetRef);
		CurrentWidgetRef->AddToViewport();
		CurrentWidgetRef->SetFocus();
	}
}

void URTUPlayerHUD::RemoveInventory()
{
	CurrentWidgetRef->RemoveFromParent();
}
