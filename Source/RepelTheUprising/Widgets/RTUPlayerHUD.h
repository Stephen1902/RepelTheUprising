// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUPlayerHUD.generated.h"

class URTUInventoryComponent;
class URTUContainerWidget;
class URTUInventoryWidget;

/**
 * 
 */
UCLASS()
class REPELTHEUPRISING_API URTUPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddInventoryToHUD();
	void AddContainerToHUD(const TSubclassOf<UUserWidget>& WidgetToDisplay, URTUInventoryComponent* ContainerInventory, URTUInventoryComponent* PlayerInventory);
	void RemoveCurrentWidget();
protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> PlayerCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> ContainerWidget;
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidgetRef;
};
