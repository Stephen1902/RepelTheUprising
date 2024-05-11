// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUInventoryGrid.generated.h"

class UBorder;
class UScrollBox;
class UWrapBox;
class URTUInventoryComponent;

UCLASS()
class REPELTHEUPRISING_API URTUInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetReferences(URTUInventoryComponent* InventoryComponentIN);
	void PopulateWrapBox();
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> GridBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UScrollBox> GridScrollBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UWrapBox> GridWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot Widget")
	TSubclassOf<class URTUInventorySlot> SlotWidget;
private:
	UPROPERTY()
	TObjectPtr<URTUInventoryComponent> InventoryCompRef;

};