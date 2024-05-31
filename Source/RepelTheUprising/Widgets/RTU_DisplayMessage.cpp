// Copyright 2024 DME Games

#include "RTU_DisplayMessage.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void URTU_DisplayMessage::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (UObject* FontObj = LoadObject<UObject>(this, TEXT("/Script/Engine.Font'/Game/UI/Fonts/MenuFont.MenuFont'"), nullptr, LOAD_None, nullptr))
	{
		// Set the font for your UTextBlock
		FSlateFontInfo FontInfo;
		FontInfo.FontObject = FontObj;
		FontInfo.Size = 14;

		MessageText->SetFont(FontInfo);
	}

	SetMessageText(FText::FromString(""));
}

void URTU_DisplayMessage::SetMessageText(const FText& TextIn)
{
	if (TextIn.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Hidden);	
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MessageText->SetText(TextIn);
	}
}
