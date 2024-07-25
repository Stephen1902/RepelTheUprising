// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTUFoodComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFoodChanged, URTUFoodComponent*, OwningFoodComp, int32, FoodToChange, int32, FoodDelta);
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxFoodChanged, int32, MaxFood);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWidgetUpdate, FText, CurrentFood, double, FoodAsPercentage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUFoodComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTUFoodComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing=OnRep_Food, BlueprintReadOnly, Category = "Food Component")
	int32 WholeFood;

	UPROPERTY(ReplicatedUsing=OnRep_FoodValue, BlueprintReadOnly, Category = "Food Component")
	int32 Food;

	UFUNCTION()
	void OnRep_Food(int32 OldFood);

	UPROPERTY(ReplicatedUsing=OnRep_MaxFood, BlueprintReadOnly, Category = "Food Component")
	int32 MaxFood;

	UFUNCTION()
	void OnRep_MaxFood(int32 OldMaxFood);

	UFUNCTION()
	void OnRep_FoodValue(int32 OldFood);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food Component")
	float StartingFood;

	// Food drain per second when owner is not using extra energy by sprinting, mining etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food Component")
	double FoodDrainStandard;

	// Food drain per second when owner is using extra energy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food Component")
	double FoodDrainExtra;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 GetFood() const;
	int32 GetMaxFood() const { return MaxFood; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFoodChanged OnFoodChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMaxFoodChanged OnMaxFoodChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWidgetUpdate OnWidgetUpdate;
	
	UFUNCTION(BlueprintCallable, Category = "Food Component")
	void ConsumeFood(const int32 FoodAmount);

private:
	bool bUsingExtraEnergy;

	UFUNCTION()
	void SprintStatusChanged(bool NewStatusIn);
	void SetNewFoodValue(const int32 NewFoodValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetNewFoodValue(const int32 NewFoodValue);
	
	double FoodAsDouble;
};

