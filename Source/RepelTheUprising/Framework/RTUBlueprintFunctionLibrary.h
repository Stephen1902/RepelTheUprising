// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTUBlueprintFunctionLibrary.generated.h"

/**
 * 
 */

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
};

UCLASS()
class REPELTHEUPRISING_API URTUBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
