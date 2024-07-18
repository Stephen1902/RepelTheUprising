// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUPlayerStatus.generated.h"

class USizeBox;
class UHorizontalBox;
class UVerticalBox;
class UBorder;
class UOverlay;
class UTextBlock;
class UImage;
class URTUInventoryGrid;
class URTUInventorySlot;
class UProgressBar;

/**
 * A class for the player status, including a hot bar
 */
UCLASS()
class REPELTHEUPRISING_API URTUPlayerStatus : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetReferences(class URTUInventoryComponent* InventoryComponentIN);

	void HotBarValueChanged(const int32 HotBarValue);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> StatusHorizBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> StatusHealthStaminaBorder;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> StatusHealthStaminaHorizBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UOverlay> StatusHealthOverlay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> StatusHealthBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> StatusHealthInfoBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> StatusHealthText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> StatusHealthImage;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UOverlay> StatusStaminaOverlay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> StatusStaminaBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> StatusStaminaText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> StatusStaminaImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> StatusXPHotBarBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> StatusXPHotBarBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> StatusXPBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<URTUInventoryGrid> StatusHotBarGrid;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> StatusFoodWaterBorder;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> StatusFoodWaterHorizBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UOverlay> StatusFoodOverlay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> StatusFoodBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> StatusFoodText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> StatusFoodImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UOverlay> StatusWaterOverlay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> StatusWaterBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> StatusWaterText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> StatusWaterImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	URTUInventoryComponent* InventoryComponentRef;

private:
	virtual void NativeConstruct() override;

	UPROPERTY()
	URTUInventorySlot* HighlightedSlot;
};
