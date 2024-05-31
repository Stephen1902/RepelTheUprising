// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTU_DisplayMessage.generated.h"

/**
 * 
 */
UCLASS()
class REPELTHEUPRISING_API URTU_DisplayMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMessageText(const FText& TextIn);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UCanvasPanel* MessagePanel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UBorder* MessageBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* MessageText;

private:
	virtual void NativePreConstruct() override;
};
