// Copyright 2024 DME Games


#include "RTUPlayerHUD.h"
#include "RTUContainerWidget.h"

void URTUPlayerHUD::AddInventoryToHUD()
{
	if (InventoryWidget)
	{
		if (PlayerWidgetRef == nullptr)
		{
			PlayerWidgetRef = CreateWidget(GetOwningPlayer(), InventoryWidget);
			PlayerWidgetRef->AddToViewport();
		}
		else
		{
			PlayerWidgetRef->SetVisibility(ESlateVisibility::Visible);
		}
		//CurrentWidgetRef->SetFocus();
	}
}

void URTUPlayerHUD::RemoveAllWidgets()
{
	if (PlayerWidgetRef)
	{
		PlayerWidgetRef->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ContainerWidgetRef)
	{
		ContainerWidgetRef->RemoveFromParent();
		ContainerWidgetRef = nullptr;
	}
	
}

void URTUPlayerHUD::AddContainerToHUD(const TSubclassOf<UUserWidget>& WidgetToDisplay, URTUInventoryComponent* ContainerInventory, URTUInventoryComponent* PlayerInventory)
{
	if (WidgetToDisplay && ContainerWidgetRef == nullptr)
	{
		ContainerWidgetRef = CreateWidget<URTUContainerWidget>(GetOwningPlayer(), WidgetToDisplay);
		ContainerWidgetRef->AddToViewport();
		ContainerWidgetRef->SetInventoryComp(ContainerInventory, PlayerInventory);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Container Widget REF is %s"), ContainerWidgetRef == nullptr? TEXT("nullptr") : TEXT("Valid"));
	}
}

void URTUPlayerHUD::RemoveCurrentWidget()
{
	if (PlayerWidgetRef != nullptr)
	{
		PlayerWidgetRef->SetVisibility(ESlateVisibility::Hidden);
		//CurrentWidgetRef = nullptr;
	}
}

