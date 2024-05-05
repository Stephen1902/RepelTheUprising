// Copyright 2024 DME Games


#include "RTUFoodComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URTUFoodComponent::URTUFoodComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URTUFoodComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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

	// ...
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

void URTUFoodComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUFoodComponent, Food);
}