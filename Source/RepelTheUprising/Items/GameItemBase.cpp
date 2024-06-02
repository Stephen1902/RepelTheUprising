// Copyright 2024 DME Games

#include "GameItemBase.h"

#include "RepelTheUprising/Components/RTUItemComponent.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"

#define InteractTrace ECC_GameTraceChannel2

// Sets default values
AGameItemBase::AGameItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComponent->SetupAttachment(SceneComponent);
	MeshComponent->SetCollisionResponseToChannel(InteractTrace, ECR_Block);

	ItemComponent = CreateDefaultSubobject<URTUItemComponent>(TEXT("Item Component"));
	
	TextToReturn = FText::FromString("");
	
}

// Called when the game starts or when spawned
void AGameItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FText AGameItemBase::LookAt_Implementation()
{
	if (ItemComponent)
	{
		FDataTableRowHandle ItemID = ItemComponent->GetItemID();
		const UDataTable* ItemTable = ItemID.DataTable;

		if (FItemInformationTable* Row = ItemTable->FindRow<FItemInformationTable>(ItemID.RowName, ""))
		{
			FString StringToReturn = "Pick up ";
			const int32 LocalQty = ItemComponent->GetQuantity();
			if (LocalQty > 1)
			{
				StringToReturn += FString::FromInt(LocalQty) + " " += Row->ItemName.ToString() + "s";
			}
			else
			{
			  StringToReturn += Row->ItemName.ToString();	
			}
			
			TextToReturn = FText::FromString(StringToReturn);
		}
	}
	return TextToReturn;	
}

void AGameItemBase::SetItemInfo(const FName INItemID, const int32 InQuantity)
{
	if (INItemID != "" && InQuantity > 0)
	{
		ItemComponent->SetItemInfo(INItemID, InQuantity);
	}
}

