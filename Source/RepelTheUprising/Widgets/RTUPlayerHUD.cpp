// Copyright 2024 DME Games


#include "RTUPlayerHUD.h"
#include "RTUContainerWidget.h"

void URTUPlayerHUD::AddInventoryToHUD()
{
	if (InventoryWidget)
	{
		CurrentWidgetRef = CreateWidget(GetOwningPlayer(), InventoryWidget);
		CurrentWidgetRef->AddToViewport();
		CurrentWidgetRef->SetFocus();
	}
}

void URTUPlayerHUD::AddContainerToHUD(const TSubclassOf<UUserWidget>& WidgetToDisplay, URTUInventoryComponent* ContainerInventory, URTUInventoryComponent* PlayerInventory)
{
	if (WidgetToDisplay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid widget passed to Player HUD"));
		CurrentWidgetRef = CreateWidget(GetOwningPlayer(), WidgetToDisplay);
		CurrentWidgetRef->AddToViewport();
		URTUContainerWidget* ContainerWidgetRef = Cast<URTUContainerWidget>(CurrentWidgetRef);
		ContainerWidgetRef->SetInventoryComp(ContainerInventory, PlayerInventory);
		CurrentWidgetRef->SetFocus();		
	}
}

void URTUPlayerHUD::RemoveCurrentWidget()
{
	if (CurrentWidgetRef != nullptr)
	{
		CurrentWidgetRef->RemoveFromParent();
		CurrentWidgetRef = nullptr;
	}
}

