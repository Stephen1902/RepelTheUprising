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


UCLASS()
class REPELTHEUPRISING_API URTUBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
