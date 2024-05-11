// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTUPlayerWidget.generated.h"

UCLASS()
class REPELTHEUPRISING_API URTUPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetReferences(class URTUInventoryComponent* InventoryComponentIN);
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> PlayerCanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class URTUInventoryGrid> PlayerInventoryGrid;

private:
	UPROPERTY()
	URTUInventoryComponent* InventoryComponentRef;
};
