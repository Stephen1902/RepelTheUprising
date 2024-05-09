// Copyright 2024 DME Games

#include "RTUItemComponent.h"

#include "RTUInventoryComponent.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"

// Sets default values for this component's properties
URTUItemComponent::URTUItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableInfo(TEXT("'/Game/Items/DT_ItemInfo'"));
	if (DataTableInfo.Succeeded())
	{
		ItemID.DataTable = DataTableInfo.Object;
	}

	Quantity = 1;
}


// Called when the game starts
void URTUItemComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->SetReplicates(true);
	
}


void URTUItemComponent::InteractWith_Implementation(ARepelTheUprisingCharacter* CharacterWhoInteracted)
{	
	if (CharacterWhoInteracted)
	{
		if (URTUInventoryComponent* InventoryComponent = CharacterWhoInteracted->GetInventoryComp())
		{
			InventoryComponent->AddToInventory(ItemID.RowName, Quantity, Quantity);
		}
	}
	
	if (Quantity == 0)
	{
		GetOwner()->Destroy();
	}
}

// Called every frame
void URTUItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

