// Copyright 2024 DME Games


#include "RTUPlayerStatus.h"

#include "RTUInventoryGrid.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"
#include "RepelTheUprising/Player/RepelTheUprisingPlayerController.h"

void URTUPlayerStatus::SetReferences(URTUInventoryComponent* InventoryComponentIN)
{
	if (InventoryComponentIN != nullptr)
	{
		InventoryComponentRef = InventoryComponentIN;
	}
}

void URTUPlayerStatus::HotBarValueChanged(const int32 HotBarValue)
{
	UE_LOG(LogTemp, Warning, TEXT("HotBarValue is %i"), HotBarValue);
	if (HighlightedSlot != nullptr)
	{
		HighlightedSlot->UpdateHotBarHighlight(false);
	}

	HighlightedSlot = Cast<URTUInventorySlot>(StatusHotBarGrid->GetGridWrapBox()->GetChildAt(HotBarValue));

	if (HighlightedSlot)
	{
		HighlightedSlot->UpdateHotBarHighlight(true);
	}
}


void URTUPlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComponentRef == nullptr)
	{
		const ARepelTheUprisingCharacter* PlayerChar = Cast<ARepelTheUprisingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		APlayerController* PlayerController = Cast<APlayerController>(PlayerChar->GetController());
/*		if (PlayerController)
		{
			SetOwningPlayer(PlayerController);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get player controller"));
		}*/
		InventoryComponentRef = PlayerChar->GetHotBarComp();
		StatusHotBarGrid->SetReferences(InventoryComponentRef);
	}

}
