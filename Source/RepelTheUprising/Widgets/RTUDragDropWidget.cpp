// Copyright 2024 DME Games


#include "RTUDragDropWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RepelTheUprising/Framework/RTUBlueprintFunctionLibrary.h"


void URTUDragDropWidget::SetInformation(const FName INItemID, const int32 INQuantity)
{
	ItemID = INItemID;
	Quantity = INQuantity;

	if (ItemInfoTable)
	{
		if (const FItemInformationTable* Row = ItemInfoTable->FindRow<FItemInformationTable>(ItemID, ""))
		{
			DDImage->SetBrushFromTexture(Row->ItemThumbnail, false);
			DDImage->SetVisibility(ESlateVisibility::Visible);
			DDQtyTextBox->SetText(FText::FromString(FString::FromInt(Quantity)));

			if (UObject* FontObj = LoadObject<UObject>(GetWorld(), TEXT("/Script/Engine.Font'/Game/UI/Fonts/MenuFont.MenuFont'"), nullptr, LOAD_None, nullptr))
			{
				// Set the font for your UTextBlock
				FSlateFontInfo FontInfo;
				FontInfo.FontObject = FontObj;
		
				// Adjust the font size, depending on how large the quantity is
				FontInfo.Size = 8;

				if (Quantity > 9)
				{
					if (Quantity < 100)
					{
						FontInfo.Size = 7;
					}
					if (Quantity > 99 && Quantity < 1000)
					{
						FontInfo.Size = 6;
					}
					if (Quantity > 999)
					{
						FontInfo.Size = 4.5;
					}
				}
		
				DDQtyTextBox->SetFont(FontInfo);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Action Menu Widget tried to get MenuFont but was unable to do so."))
			}
			
			DDQtySizeBox->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			DDImage->SetVisibility(ESlateVisibility::Hidden);
			DDQtySizeBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void URTUDragDropWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemInfoTable)
	{
		if (const FItemInformationTable* Row = ItemInfoTable->FindRow<FItemInformationTable>(ItemID, ""))
		{
			DDImage->SetBrushFromTexture(Row->ItemThumbnail, false);
		}
	}

	OwningController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void URTUDragDropWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwningController)
	{
		float MouseX;
		float MouseY;
		OwningController->GetMousePosition(MouseX, MouseY);
		const FVector2D MousePosition = UKismetMathLibrary::MakeVector2D(MouseX, MouseY);
		SetPositionInViewport(MousePosition);
	}
}
