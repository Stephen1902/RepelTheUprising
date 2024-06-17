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
	FName GetItemID() const { return ItemID; }
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TSubclassOf<class URTUDragDropWidget> DragDropWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TSubclassOf<class URTUActionMenuWidget> ActionMenuWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TObjectPtr<URTUInventoryComponent> InventoryCompRef = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	FName ItemID = FName("");
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	int32 Quantity = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	int32 ContentIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	bool bIsRightMouseButton = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	UDataTable* ItemTable;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void DealWithRightMouseDrop(FName InItemID, int32 InQuantity);
	
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

private:
	UPROPERTY()
	TObjectPtr<URTUActionMenuWidget> ActionMenuWidgetRef;

	UFUNCTION()
	void OnButtonHovered();
	UFUNCTION()
	void OnButtonUnhovered();
};