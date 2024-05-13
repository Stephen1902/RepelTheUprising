// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "RTUDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class REPELTHEUPRISING_API URTUDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn), Category = "Drag Drop Operations")
	TObjectPtr<class URTUInventoryComponent> InventoryComponentRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn), Category = "Drag Drop Operations")
	int32 ContentIndex;
	
};
