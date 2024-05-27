// Copyright 2024 DME Games

#include "RTUContainerWidget.h"
#include "RTUInventoryGrid.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"

void URTUContainerWidget::SetInventoryComp(URTUInventoryComponent* ContainerInventoryIn, URTUInventoryComponent* PlayerInventoryIn)
{
	if (ContainerInventoryIn && PlayerInventoryIn)
	{
		ContainerInventoryRef = ContainerInventoryIn;
		CWContainerWidget->SetReferences(ContainerInventoryRef);
		
		PlayerInventoryRef = PlayerInventoryIn;
		CWPlayerWidget->SetReferences(PlayerInventoryRef);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryComp was called on the Container Widget but without a valid Container or Player inventory"));
	}
	
}
