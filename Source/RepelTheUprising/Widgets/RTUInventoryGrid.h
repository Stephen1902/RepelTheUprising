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
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetReferences(URTUInventoryComponent* InventoryComponentIN);
	UFUNCTION()
	void PopulateWrapBox();
	UWrapBox* GetGridWrapBox() const { return GridWrapBox; }
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> GridBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UScrollBox> GridScrollBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UWrapBox> GridWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot Widget")
	TSubclassOf<class URTUInventorySlot> SlotWidget;

	UFUNCTION(Client, Reliable)
	void ClientUpdateInventory();
private:
	UPROPERTY()
	TObjectPtr<URTUInventoryComponent> InventoryCompRef;

};
