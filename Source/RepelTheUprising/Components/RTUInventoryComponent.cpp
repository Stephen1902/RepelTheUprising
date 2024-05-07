// Copyright 2024 DME Games


#include "RTUInventoryComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URTUInventoryComponent::URTUInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SetIsReplicatedByDefault(true);
}

void URTUInventoryComponent::AddToInventory()
{
	
}

void URTUInventoryComponent::RemoveFromInventory()
{
	
}

void URTUInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUInventoryComponent, SlotStruct);
}
