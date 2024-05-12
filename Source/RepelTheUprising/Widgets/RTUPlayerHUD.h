// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class REPELTHEUPRISING_API URTUPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddInventoryToHUD();
	void RemoveInventory();
protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> PlayerCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class URTUInventoryWidget> InventoryWidgetRef;
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidgetRef;
};
