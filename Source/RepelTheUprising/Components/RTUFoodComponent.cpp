// Copyright 2024 DME Games


#include "RTUFoodComponent.h"
#include "RTUStaminaComponent.h"
#include "DSP/AudioFFT.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URTUFoodComponent::URTUFoodComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	StartingFood = 100;
	MaxFood = StartingFood;
	WholeFood = 100;
	Food = StartingFood - 10;
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
			StaminaCompRef->OnSprintingChanged.AddDynamic(this, &URTUFoodComponent::URTUFoodComponent::SprintStatusChanged);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get Stamina Component reference in Food Component for %s"), *GetName());
		}
	}

	FoodAsDouble = Food;
	
	// Send the new information to the widget
	const double MaxFoodAsDouble = MaxFood; 
	const double FoodAsPercentage = 1.0 - ((MaxFoodAsDouble - FoodAsDouble) / MaxFoodAsDouble);
	UE_LOG(LogTemp, Warning, TEXT("FoodAsPercentage: %s, Food: %i, MaxFood: %i"), *FString::SanitizeFloat(FoodAsPercentage), WholeFood, MaxFood);
	OnWidgetUpdate.Broadcast(FText::FromString(FString::FromInt(WholeFood)), FoodAsPercentage);
	if (GetOwner()->HasAuthority())
	{
		OnRep_Food(0);
	}
}

void URTUFoodComponent::OnRep_Food(int32 OldFood)
{
	const int32 NewFood = WholeFood + OldFood;

	OnFoodChanged.Broadcast(this, WholeFood, NewFood);

	// Send the new information to the widget
	const double MaxFoodAsDouble = MaxFood; 
	const double FoodAsPercentage = 1.0 - ((MaxFoodAsDouble - FoodAsDouble) / MaxFoodAsDouble);
	OnWidgetUpdate.Broadcast(FText::FromString(FString::FromInt(WholeFood)), FoodAsPercentage);
}

void URTUFoodComponent::OnRep_MaxFood(int32 OldMaxFood)
{
	OnMaxFoodChanged.Broadcast(MaxFood);
}

void URTUFoodComponent::OnRep_FoodValue(int32 OldFood)
{
	UE_LOG(LogTemp, Warning, TEXT("Repping FoodValue"));	
}

// Called every frame
void URTUFoodComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const int32 OldFood = FMath::Floor(FoodAsDouble);
	
	if (!bUsingExtraEnergy)
	{
		FoodAsDouble = FMath::Clamp(FoodAsDouble - (FoodDrainStandard * DeltaTime), 0.f, MaxFood);
	}
	else
	{
		FoodAsDouble = FMath::Clamp(FoodAsDouble - (FoodDrainExtra * DeltaTime), 0.f, MaxFood);
	}

	const int32 FoodAsInt = FMath::Floor(FoodAsDouble);
	// If the value of food has changed, set the replicated variable to use it
	if (OldFood != FoodAsInt)
	{
		SetNewFoodValue(FoodAsInt);
		// On Rep functions have to manually called on the server, called automatically on the client
		if (GetOwner()->HasAuthority())
		{
		//	OnRep_Food(0);
		}
	}

}

int32 URTUFoodComponent::GetFood() const
{
	return WholeFood;
}


void URTUFoodComponent::ConsumeFood(const int32 FoodAmount)
{
	if (FoodAmount <= 0)
	{
		return;
	}

	WholeFood = FMath::Clamp(WholeFood + FoodAmount, 0.f, MaxFood);
	UE_LOG(LogTemp, Warning, TEXT("Food is now %i"), WholeFood);

	OnFoodChanged.Broadcast(this, WholeFood, -FoodAmount);
}

void URTUFoodComponent::SprintStatusChanged(bool NewStatusIn)
{
	bUsingExtraEnergy = NewStatusIn;
}

void URTUFoodComponent::SetNewFoodValue(const int32 NewFoodValue)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SetNewFoodValue(NewFoodValue);
		return;
	}

	WholeFood = NewFoodValue;
}

void URTUFoodComponent::Server_SetNewFoodValue_Implementation(const int32 NewFoodValue)
{
	SetNewFoodValue(NewFoodValue);
}

bool URTUFoodComponent::Server_SetNewFoodValue_Validate(const int32 NewFoodValue)
{
	return GetOwner() != nullptr;
}


void URTUFoodComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUFoodComponent, WholeFood);
	DOREPLIFETIME(URTUFoodComponent, MaxFood);
	DOREPLIFETIME(URTUFoodComponent, Food);
}
