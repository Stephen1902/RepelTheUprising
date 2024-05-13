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
	void SetItemID(const FName INItemID);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class USizeBox> DDSizeBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UBorder> DDBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UImage> DDImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drag Drop")
	TObjectPtr<UDataTable> ItemInfoTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn), Category = "Drag Drop")
	FName ItemID;

private:
	virtual void NativeConstruct() override;
};
