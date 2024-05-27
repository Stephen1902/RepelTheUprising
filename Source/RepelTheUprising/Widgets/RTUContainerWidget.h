// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUContainerWidget.generated.h"

class URTUInventoryGrid;
class URTUInventoryComponent;

UCLASS()
class REPELTHEUPRISING_API URTUContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInventoryComp(URTUInventoryComponent* ContainerInventoryIn, URTUInventoryComponent* PlayerInventoryIn);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> CWCanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<URTUInventoryGrid> CWContainerWidget;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<URTUInventoryGrid> CWPlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drag Drop")
	TObjectPtr<UDataTable> ItemInfoTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn), Category = "Drag Drop")
	FName ItemID;

private:
	TObjectPtr<URTUInventoryComponent> ContainerInventoryRef;
	TObjectPtr<URTUInventoryComponent> PlayerInventoryRef; 
};