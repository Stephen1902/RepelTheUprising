// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUDragDropWidget.generated.h"

/**
 * 
 */
UCLASS()
class REPELTHEUPRISING_API URTUDragDropWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInformation(const FName INItemID, const int32 INQuantity);
	//void GetInformation() { FName& OUTItemID = FName(""), int32& OUTQuantity};
	FName GetItemID() const { return ItemID; }
	int32 GetQuantity() const { return Quantity; }
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> DDCanvasPanel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class USizeBox> DDSizeBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UBorder> DDBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UOverlay> DDOverlay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UImage> DDImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USizeBox> DDQtySizeBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> DDQtyOuterBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> DDQtyInnerBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UTextBlock> DDQtyTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drag Drop")
	TObjectPtr<UDataTable> ItemInfoTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn), Category = "Drag Drop")
	FName ItemID;

private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	TObjectPtr<APlayerController> OwningController;

	int32 Quantity;
};
