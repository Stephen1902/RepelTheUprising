// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUInventorySlot.generated.h"

class USizeBox;
class UButton;
class UBorder;
class UOverlay;
class UImage;
class UTextBlock;

UCLASS()
class REPELTHEUPRISING_API URTUInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	bool SetReferences(class URTUInventoryComponent* INInventoryCompRef, FName INItemID, int32 INQuantity, int32 INContentIndex);
	void UpdateItemSlot() const;
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USizeBox> OuterSizeBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> SlotButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> OuterBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> InnerBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UOverlay> SlotOverlay;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USizeBox> QtySizeBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> QtyOuterBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> QtyInnerBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> QtyTextBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TObjectPtr<URTUInventoryComponent> InventoryCompRef = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	FName ItemID = FName("");
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	int32 Quantity = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	int32 ContentIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	UDataTable* ItemTable;

};