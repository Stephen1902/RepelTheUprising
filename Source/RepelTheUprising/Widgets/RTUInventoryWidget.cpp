// Copyright 2024 DME Games

#include "RTUInventoryWidget.h"
#include "RTUInventoryGrid.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"
#include "RepelTheUprising/Player/RepelTheUprisingPlayerController.h"

void URTUInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const ARepelTheUprisingCharacter* PlayerChar = Cast<ARepelTheUprisingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (InventoryComponentRef == nullptr)
	{
		InventoryComponentRef = PlayerChar->GetInventoryComp();
	}

	PlayerInventoryGrid->SetReferences(InventoryComponentRef);

	if (!PlayerChar || !InventoryComponentRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get either PlayerChar or InventoryComponentRef in URTUInventoryWidget"));
	}

	PlayerButton->OnHovered.AddDynamic(this, &URTUInventoryWidget::DealWithHoveredButton);
}

void URTUInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}

void URTUInventoryWidget::DealWithHoveredButton()
{
	Execute_InventorySlotHovered(Cast<ARepelTheUprisingPlayerController>(GetOwningPlayer()), nullptr);
}

void URTUInventoryWidget::SetReferences(URTUInventoryComponent* InventoryComponentIN)
{
	if (InventoryComponentIN != nullptr)
	{
		InventoryComponentRef = InventoryComponentIN;
	}
}
