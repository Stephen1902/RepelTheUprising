// Copyright 2024 DME Games

#include "RTUInventoryWidget.h"
#include "RTUInventoryGrid.h"
#include "Kismet/GameplayStatics.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"

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

void URTUInventoryWidget::SetReferences(URTUInventoryComponent* InventoryComponentIN)
{
	if (InventoryComponentIN != nullptr)
	{
		InventoryComponentRef = InventoryComponentIN;
	}
}
