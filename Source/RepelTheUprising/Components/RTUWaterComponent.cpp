// Copyright 2024 DME Games


#include "RTUWaterComponent.h"
#include "RTUStaminaComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URTUWaterComponent::URTUWaterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	StartingWater = 100.f;
	MaxWater = StartingWater;
	Water = StartingWater;
	WaterDrainStandard = 0.02f;
	WaterDrainExtra = 0.03f;
	bUsingExtraEnergy = false;
}

// Called when the game starts
void URTUWaterComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AActor* MyOwner = GetOwner())
	{
		if (URTUStaminaComponent* StaminaCompRef = Cast<URTUStaminaComponent>(MyOwner->GetComponentByClass(URTUStaminaComponent::StaticClass())))
		{
			StaminaCompRef->OnSprintingChanged.AddDynamic(this, &URTUWaterComponent::URTUWaterComponent::SprintStatusChanged);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get Stamina Component reference in Water Component for %s"), *GetName());
		}
	}
}

void URTUWaterComponent::OnRep_Water(float OldWater)
{
	const float NewWater = Water + OldWater;

	OnWaterChanged.Broadcast(this, Water, NewWater);
}

// Called every frame
void URTUWaterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bUsingExtraEnergy)
	{
		Water = FMath::Clamp(Water - (WaterDrainStandard * DeltaTime), 0.f, MaxWater);
	}
	else
	{
		Water = FMath::Clamp(Water - (WaterDrainExtra * DeltaTime), 0.f, MaxWater);
	}
}

float URTUWaterComponent::GetWater() const
{
	return Water;
}

void URTUWaterComponent::ConsumeWater(const float WaterAmount)
{
	if (WaterAmount <= 0.0f)
	{
		return;
	}

	Water = FMath::Clamp(Water + WaterAmount, 0.f, StartingWater);

	OnWaterChanged.Broadcast(this, Water, -WaterAmount);
}

void URTUWaterComponent::SprintStatusChanged(bool NewStatusIn)
{
	bUsingExtraEnergy = NewStatusIn;
	
	UE_LOG(LogTemp, Warning, TEXT("%s has Water of :%s"), *GetName(), *FString::SanitizeFloat(Water));
}

void URTUWaterComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUWaterComponent, Water);
}
