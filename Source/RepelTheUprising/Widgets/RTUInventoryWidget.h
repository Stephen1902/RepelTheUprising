// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUInventoryWidget.generated.h"

UCLASS()
class REPELTHEUPRISING_API URTUInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetReferences(class URTUInventoryComponent* InventoryComponentIN);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> PlayerCanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class URTUInventoryGrid> PlayerInventoryGrid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	URTUInventoryComponent* InventoryComponentRef;
};
