// Copyright 2024 DME Games


#include "RTUFoodComponent.h"
#include "RTUStaminaComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URTUFoodComponent::URTUFoodComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	StartingFood = 100.f;
	MaxFood = StartingFood;
	Food = StartingFood;
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
}

void URTUFoodComponent::OnRep_Food(float OldFood)
{
	const float NewFood = Food + OldFood;

	OnFoodChanged.Broadcast(this, Food, NewFood);
}

// Called every frame
void URTUFoodComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bUsingExtraEnergy)
	{
		Food = FMath::Clamp(Food - (FoodDrainStandard * DeltaTime), 0.f, MaxFood);
	}
	else
	{
		Food = FMath::Clamp(Food - (FoodDrainExtra * DeltaTime), 0.f, MaxFood);
	}
}

float URTUFoodComponent::GetFood() const
{
	return Food;
}

void URTUFoodComponent::ConsumeFood(const float FoodAmount)
{
	if (FoodAmount <= 0.0f)
	{
		return;
	}

	Food = FMath::Clamp(Food + FoodAmount, 0.f, StartingFood);

	OnFoodChanged.Broadcast(this, Food, -FoodAmount);
}

void URTUFoodComponent::SprintStatusChanged(bool NewStatusIn)
{
	bUsingExtraEnergy = NewStatusIn;
}

void URTUFoodComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUFoodComponent, Food);
}
