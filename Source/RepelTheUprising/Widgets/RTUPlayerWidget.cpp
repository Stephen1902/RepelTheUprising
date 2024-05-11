// Copyright 2024 DME Games

#include "RTUPlayerWidget.h"
#include "RTUInventoryGrid.h"

void URTUPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerInventoryGrid && InventoryComponentRef)
	{
		//PlayerInventoryGrid->SetReferences(InventoryComponentRef);
	}
}

void URTUPlayerWidget::SetReferences(URTUInventoryComponent* InventoryComponentIN)
{
	if (InventoryComponentIN != nullptr)
	{
		InventoryComponentRef = InventoryComponentIN;
		PlayerInventoryGrid->SetReferences(InventoryComponentRef);
	}
}
