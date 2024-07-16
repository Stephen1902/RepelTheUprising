// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RTUInventorySlot.generated.h"

class USizeBox;
class UButton;
class UBorder;
class UOverlay;
class UImage;
class UTextBlock;

USTRUCT(Blueprintable)
struct FInventorySlotStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TObjectPtr<URTUInventoryComponent> InventoryCompRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	FName ItemID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	int32 ContentIndex;

	FInventorySlotStruct()
	{
		InventoryCompRef = nullptr;
		ItemID = FName("");
		Quantity = -1;
		ContentIndex = -1;
	}
};

UCLASS()
class REPELTHEUPRISING_API URTUInventorySlot : public UUserWidget, public IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	bool SetReferences(class URTUInventoryComponent* INInventoryCompRef, FName INItemID, int32 INQuantity, int32 INContentIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void UpdateItemSlot() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	FInventorySlotStruct GetInventorySlotStruct() const { return InventorySlotStruct; }

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void DealWithMouseDrop(FName InItemID, int32 InQuantity);

	void GetSlotParent();
protected:
	virtual void NativePreConstruct() override;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TSubclassOf<class URTUDragDropWidget> DragDropWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TSubclassOf<class URTUActionMenuWidget> ActionMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	FLinearColor HighlightColourInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	FLinearColor HighlightColourHotBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	bool bIsRightMouseButton = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	UDataTable* ItemTable;
	
	//virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

private:
	UPROPERTY()
	TObjectPtr<URTUActionMenuWidget> ActionMenuWidgetRef;

	UPROPERTY()
	FInventorySlotStruct InventorySlotStruct;

	UFUNCTION()
	void OnButtonHovered();
	UFUNCTION()
	void OnButtonUnhovered();
	UFUNCTION()
	void OnButtonPressed();
	UFUNCTION()
	void OnButtonReleased();

	FLinearColor DefaultHighlightColour;
};