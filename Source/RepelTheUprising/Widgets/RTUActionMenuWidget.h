// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUActionMenuWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class REPELTHEUPRISING_API URTUActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetReferences(class URTUInventoryComponent* InventoryComponentIN, int32 IndexIN);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> MenuCanvasPanel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UBackgroundBlur> MenuBackgroundBlur;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UVerticalBox> MenuVerticalBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> MenuButtonUse;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> MenuTextUse;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> MenuButtonDropOne;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> MenuTextDropOne;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> MenuButtonDropAll;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> MenuTextDropAll;
	
private:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UPROPERTY()
	TObjectPtr<class URTUInventoryComponent> InventoryComponentRef;
	
	int32 ContentIndex;

	UFUNCTION()
	void UseButtonClicked();

	UFUNCTION()
	void DropOneButtonClicked();

	UFUNCTION()
	void DropAllButtonClicked();
};
