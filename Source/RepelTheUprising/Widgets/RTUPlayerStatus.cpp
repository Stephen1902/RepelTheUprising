// Copyright 2024 DME Games


#include "RTUPlayerStatus.h"

#include "RTUInventoryGrid.h"
#include "Kismet/GameplayStatics.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"

void URTUPlayerStatus::SetReferences(URTUInventoryComponent* InventoryComponentIN)
{
	if (InventoryComponentIN != nullptr)
	{
		InventoryComponentRef = InventoryComponentIN;
	}
}

void URTUPlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (InventoryComponentRef == nullptr)
	{
		const ARepelTheUprisingCharacter* PlayerChar = Cast<ARepelTheUprisingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		InventoryComponentRef = PlayerChar->GetHotBarComp();
		StatusHotBarGrid->SetReferences(InventoryComponentRef);
	}
}
