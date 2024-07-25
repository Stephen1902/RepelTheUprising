// Copyright 2024 DME Games

#include "RTUPlayerStatus.h"
#include "RTUInventoryGrid.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "RepelTheUprising/Components/RTUStaminaComponent.h"
#include "RepelTheUprising/Components/RTUHealthComponent.h"
#include "RepelTheUprising/Components/RTUFoodComponent.h"
#include "RepelTheUprising/Components/RTUWaterComponent.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"

void URTUPlayerStatus::SetReferences(URTUInventoryComponent* InventoryComponentIN)
{
	if (InventoryComponentIN != nullptr)
	{
		InventoryComponentRef = InventoryComponentIN;
	}
}

void URTUPlayerStatus::HotBarValueChanged(const int32 HotBarValue)
{
	UE_LOG(LogTemp, Warning, TEXT("HotBarValue is %i"), HotBarValue);
	if (HighlightedSlot != nullptr)
	{
		HighlightedSlot->UpdateHotBarHighlight(false);
	}

	HighlightedSlot = Cast<URTUInventorySlot>(StatusHotBarGrid->GetGridWrapBox()->GetChildAt(HotBarValue));

	if (HighlightedSlot)
	{
		HighlightedSlot->UpdateHotBarHighlight(true);
	}
}


void URTUPlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComponentRef == nullptr)
	{
		if (const ARepelTheUprisingCharacter* PlayerChar = Cast<ARepelTheUprisingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			InventoryComponentRef = PlayerChar->GetHotBarComp();
			StatusHotBarGrid->SetReferences(InventoryComponentRef);

			StaminaComponentRef = Cast<URTUStaminaComponent>(PlayerChar->GetComponentByClass(URTUStaminaComponent::StaticClass()));
			if (StaminaComponentRef)
			{
				StaminaComponentRef->OnStaminaChanged.AddDynamic(this, &URTUPlayerStatus::UpdateStaminaValue);
			}
			
			HealthComponentRef = Cast<URTUHealthComponent>(PlayerChar->GetComponentByClass(URTUHealthComponent::StaticClass()));
			if (HealthComponentRef)
			{
				HealthComponentRef->OnHealthChanged.AddDynamic(this, &URTUPlayerStatus::URTUPlayerStatus::UpdateHealthValue);
				HealthComponentRef->OnMaxHealthChanged.AddDynamic(this, &URTUPlayerStatus::UpdateMaxHealthValue);
				MaxPlayerHealth = HealthComponentRef->GetMaxHealth();
			}

			FoodComponentRef = Cast<URTUFoodComponent>(PlayerChar->GetComponentByClass(URTUFoodComponent::StaticClass()));
			if (FoodComponentRef)
			{
				FoodComponentRef->OnWidgetUpdate.AddDynamic(this, &URTUPlayerStatus::UpdateFoodWidget);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get a food component ref"));
			}

			WaterComponentRef = Cast<URTUWaterComponent>(PlayerChar->GetComponentByClass(URTUWaterComponent::StaticClass()));
			if (WaterComponentRef)
			{
				WaterComponentRef->OnWaterChanged.AddDynamic(this, &URTUPlayerStatus::URTUPlayerStatus::UpdateWaterValue);
			}
		}
	}

}

void URTUPlayerStatus::UpdateStaminaValue(double NewStaminaValue, double MaxStaminaValue)
{
	const int32 doubleAsInt = NewStaminaValue;
	const FText TextToPrint = FText::FromString(FString::FromInt(doubleAsInt));
	StatusStaminaText->SetText(TextToPrint);

	const double StaminaAsPercentage = 1.0f - ((MaxStaminaValue - NewStaminaValue) / MaxStaminaValue);
	StatusStaminaBar->SetPercent(StaminaAsPercentage);
}

void URTUPlayerStatus::UpdateHealthValue(URTUHealthComponent* OwningHealthComp, double Health, double HealthDelta,	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	const int32 DoubleAsInt = Health;
	const FText TextToPrint = FText::FromString(FString::FromInt(DoubleAsInt));
	StatusHealthText->SetText(TextToPrint);

	const double HealthAsPercentage = 1.0f - ((MaxPlayerHealth - Health) / MaxPlayerHealth);
	StatusHealthBar->SetPercent(HealthAsPercentage);
}

void URTUPlayerStatus::UpdateMaxHealthValue(double NewMaxHealthValue)
{
	MaxPlayerHealth = NewMaxHealthValue;
	const double CurrentPlayerHealth = HealthComponentRef->GetHealth();
	const double HealthAsPercentage = 1.0f - ((MaxPlayerHealth - CurrentPlayerHealth) / MaxPlayerHealth);
	StatusHealthBar->SetPercent(HealthAsPercentage);
}

void URTUPlayerStatus::UpdateFoodWidget(FText FoodAmount, double FoodAsPercentage)
{
	StatusFoodText->SetText(FoodAmount);
	StatusFoodBar->SetPercent(FoodAsPercentage);

	UE_LOG(LogTemp, Warning, TEXT("Food amount updated"));
}

void URTUPlayerStatus::UpdateWaterValue(URTUWaterComponent* OwningWaterComp, double WaterToChange, double WaterDelta)
{
	if (OwningWaterComp)
	{
		const int32 doubleAsInt = WaterToChange;
		const FText TextToPrint = FText::FromString(FString::FromInt(doubleAsInt));
		StatusWaterText->SetText(TextToPrint);

		const double MaxWater = OwningWaterComp->GetMaxWater();
		const double WaterAsPercentage = 1.0f - ((MaxWater - WaterToChange) / MaxWater);
		StatusWaterBar->SetPercent(WaterAsPercentage);
	}
}
