// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTUBlueprintFunctionLibrary.generated.h"

// What effects consuming an item in the item table has on the player
USTRUCT(BlueprintType)
struct FOnConsumeStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Consume Struct")
	double EffectOnFood;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Consume Struct")
	double EffectOnThirst;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Consume Struct")
	double EffectOnStamina;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Consume Struct")
	double EffectOnHealth;

	FOnConsumeStruct()
	{
		EffectOnFood = 0.f;
		EffectOnThirst = 0.f;
		EffectOnStamina = 0.f;
		EffectOnHealth = 0.f;
	}
};

USTRUCT(BlueprintType)
struct FItemInformationTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	UTexture2D* ItemThumbnail;

	// Actor to be created in the world then the player drops 
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	TSubclassOf<AActor> ActorOnDrop;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	FOnConsumeStruct OnConsumeStruct;
};

UENUM(BlueprintType)
enum class ECraftingLocation : uint8
{
	ECF_Self			UMETA(DisplayName="Self"),
	ECF_Campfire		UMETA(DisplayName="Campfire"),
	ECF_Oven			UMETA(DisplayName="Oven"),
	ECF_Worktable		UMETA(DisplayName="Worktable"),
	ECF_Workbench		UMETA(DisplayName="Workbench"),
	ECF_Forge			UMETA(DisplayName="Forge"),
	ECF_ChemSet			UMETA(DisplayName="Chemistry Set"),
	ECF_ChemStation		UMETA(DisplayName="Chemistry Station"),
	ECF_ConcreteMixer	UMETA(DisplayName="Concrete Mixer")
};

USTRUCT(BlueprintType)
struct FItemCraftingTable : public FTableRowBase
{
	GENERATED_BODY()

	// Unique item name
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	FName ItemName;

	// Helper description of the item, giving the player a clue what is needed to craft
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	FName ItemDescription;

	// Item to be created, must be an item in the ItemInformationTable
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	FName CraftedItem;

	// Location that this item is crafted in
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	ECraftingLocation CraftingLocation;

	// Maximum level item can be crafted to in above location
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	int32 MaxCraftingLevel;

	// Items and quantities needed to craft
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	TMap<FName, int32> ItemNeeds;

	// Time in seconds item needs to craft
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Item Info")
	double CraftingTime;

	FItemCraftingTable()
	{
		ItemName = FName("");
		ItemDescription = FName("");
		CraftedItem = FName("");
		CraftingLocation = ECraftingLocation::ECF_Self;
		MaxCraftingLevel = 999;
		CraftingTime = 1.0f;
	}
};

UCLASS()
class REPELTHEUPRISING_API URTUBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
