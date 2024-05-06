// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTUWaterComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWaterChanged, URTUWaterComponent*, OwningWaterComp, float, HealthToChange, float, HealthDelta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUWaterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTUWaterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing=OnRep_Water, BlueprintReadOnly, Category = "Water Component")
	float Water;

	UFUNCTION()
	void OnRep_Water(float OldWater);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Component")
	float StartingWater;

	// Water drain per second when owner is not using extra energy by sprinting, mining etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Component")
	float WaterDrainStandard;

	// Water drain per second when owner is using extra energy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Component")
	float WaterDrainExtra;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetWater() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaterChanged OnWaterChanged;

	UFUNCTION(BlueprintCallable, Category = "Water Component")
	void ConsumeWater(const float WaterAmount);

private:
	float MaxWater;
	bool bUsingExtraEnergy;

	UFUNCTION()
	void SprintStatusChanged(bool NewStatusIn);
};
