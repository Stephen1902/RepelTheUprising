// Copyright 2024 DME Games


#include "RTUFoodComponent.h"
#include "../Framework/RTUBlueprintFunctionLibrary.h"
#include "RTUInventoryComponent.h"
#include "RTUStaminaComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URTUFoodComponent::URTUFoodComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	StartingFood = 100;
	MaxFood = StartingFood;
	FoodAsInt = 90;
	FoodDrainStandard = 0.01f;
	FoodDrainExtra = 0.015f;
	bUsingExtraEnergy = false;
}

// Called when the game starts
void URTUFoodComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AActor* MyOwner = GetOwner())
	{
		if (URTUStaminaComponent* StaminaCompRef = Cast<URTUStaminaComponent>(MyOwner->GetComponentByClass(URTUStaminaComponent::StaticClass())))
		{
			StaminaCompRef->OnSprintingChanged.AddDynamic(this, &URTUFoodComponent::SprintStatusChanged);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get Stamina Component reference in Food Component for %s"), *GetName());
		}

		if (URTUInventoryComponent* InventoryCompRef = Cast<URTUInventoryComponent>(MyOwner->GetComponentByClass(URTUInventoryComponent::StaticClass())))
		{
			InventoryCompRef->OnItemConsumed.AddDynamic(this, &URTUFoodComponent::DealWithFoodConsumed);		
		}
	}

	GetWorld()->GetTimerManager().SetTimer(FoodDrainTimer, this, &URTUFoodComponent::DrainFood, TimerDelay, true);
	FoodAsDouble = FoodAsInt;
	
	// Send the new information to the widget
	const double MaxFoodAsDouble = MaxFood; 
	const double FoodAsPercentage = 1.0 - ((MaxFoodAsDouble - FoodAsDouble) / MaxFoodAsDouble);
	OnWidgetUpdate.Broadcast(FText::FromString(FString::FromInt(FoodAsInt)), FoodAsPercentage);
}

void URTUFoodComponent::AdjustFood(const int32 AmountToAdjust)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_AdjustFood(AmountToAdjust);
		return;
	}
	
	FoodAsInt = FMath::Clamp(FoodAsInt + AmountToAdjust, 0.f, MaxFood);
}

void URTUFoodComponent::UpdateMaxFood(const int32 AmountToAdjust)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_UpdateMaxFood(AmountToAdjust);
		return;
	}

	MaxFood += AmountToAdjust;
	// Update the player widget
	const double MaxFoodAsDouble = MaxFood; 
	const double FoodAsPercentage = 1.0 - ((MaxFoodAsDouble - FoodAsDouble) / MaxFoodAsDouble);
	OnWidgetUpdate.Broadcast(FText::FromString(FString::FromInt(FoodAsInt)), FoodAsPercentage);
}

void URTUFoodComponent::SprintStatusChanged(bool NewStatusIn)
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Status Changed"));
	if (!GetOwner()->HasAuthority())
	{
		Server_SprintStatusChanged(NewStatusIn);
		return;
	}
	
	bUsingExtraEnergy = NewStatusIn;
	UE_LOG(LogTemp, Warning, TEXT("bUsingExtraEnergy set to %s"), bUsingExtraEnergy ? TEXT("true") : TEXT("false"));
}

void URTUFoodComponent::Server_UpdateMaxFood_Implementation(const int32 AmountToAdjust)
{
	UpdateMaxFood(AmountToAdjust);
}

bool URTUFoodComponent::Server_UpdateMaxFood_Validate(const int32 AmountToAdjust)
{
	return AmountToAdjust > 0;
}

void URTUFoodComponent::Server_SprintStatusChanged_Implementation(bool NewStatusIn)
{
	SprintStatusChanged(NewStatusIn);	
}

bool URTUFoodComponent::Server_SprintStatusChanged_Validate(bool NewStatusIn)
{
	if (const AActor* MyOwner = GetOwner())
	{
		if (const URTUStaminaComponent* StaminaComponent = Cast<URTUStaminaComponent>(MyOwner->GetComponentByClass(URTUStaminaComponent::StaticClass())))
		{
			return true;
		}
	}

	return false;
}

void URTUFoodComponent::Server_AdjustFood_Implementation(const int32 AmountToAdjust)
{
	AdjustFood(AmountToAdjust);
}

bool URTUFoodComponent::Server_AdjustFood_Validate(const int32 AmountToAdjust)
{
	return AmountToAdjust > 0;
}

void URTUFoodComponent::DrainFood()
{
	const int32 OldFood = FMath::Floor(FoodAsDouble);
	
	if (!bUsingExtraEnergy)
	{
		FoodAsDouble = FMath::Clamp(FoodAsDouble - (FoodDrainStandard * TimerDelay), 0.f, MaxFood);
	}
	else
	{
		FoodAsDouble = FMath::Clamp(FoodAsDouble - (FoodDrainExtra * TimerDelay), 0.f, MaxFood);
	}

	FoodAsInt = FMath::Floor(FoodAsDouble);
	// If the value of food has changed, set the replicated variable to use it
	if (OldFood != FoodAsInt)
	{
		const double MaxFoodAsDouble = MaxFood; 
		const float FoodAsPercentage = 1.0f - ((MaxFoodAsDouble - FoodAsDouble) / MaxFoodAsDouble);
		OnWidgetUpdate.Broadcast(FText::FromString(FString::FromInt(FoodAsInt)), FoodAsPercentage);		
	}
}

void URTUFoodComponent::DealWithFoodConsumed(const FName ItemID, const UDataTable* DataTableIn)
{
	if (ItemID != FName("") && DataTableIn)
	{
		if (const FItemInformationTable* Row = DataTableIn->FindRow<FItemInformationTable>(ItemID, ""))
		{
			if (Row->OnConsumeStruct.EffectOnFood > 0)
			{
				AdjustFood(Row->OnConsumeStruct.EffectOnFood);
			}
			return;
		}
	}
	// The function has been called but without valid info.  Cheating could be being attempted.
	Server_AdjustFood(0);
}

void URTUFoodComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUFoodComponent, FoodAsInt);
	DOREPLIFETIME(URTUFoodComponent, MaxFood);
	DOREPLIFETIME(URTUFoodComponent, bUsingExtraEnergy);
}
