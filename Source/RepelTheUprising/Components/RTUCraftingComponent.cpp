// Copyright 2024 DME Games


#include "RTUCraftingComponent.h"
#include "../Player/RepelTheUprisingCharacter.h"

// Sets default values for this component's properties
URTUCraftingComponent::URTUCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CraftingSlots = 4;
}

// Called when the game starts
void URTUCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Crafting Component Begin Play called"));
	if (AActor* CompOwner = GetOwner())
	{
		OwnerInventoryComp = Execute_GetOwnerInventoryComp(CompOwner);
		if (!OwnerInventoryComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s tried to get an InventoryComp but failed from owner %s"), *GetName(), *CompOwner->GetName());
		}
	}
	// ...
	
}


// Called every frame
void URTUCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

