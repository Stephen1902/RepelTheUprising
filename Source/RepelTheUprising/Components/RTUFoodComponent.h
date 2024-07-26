// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTUFoodComponent.generated.h"

/*UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFoodChanged, URTUFoodComponent*, OwningFoodComp, int32, FoodToChange, int32, FoodDelta);
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxFoodChanged, int32, MaxFood);
*/
UDELEGATE(BlueprintAuthorityOnly)
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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Food Component")
	int32 FoodAsInt;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Food Component")
	int32 MaxFood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food Component")
	float StartingFood;

	// Food drain per second when owner is not using extra energy by sprinting, mining etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food Component")
	double FoodDrainStandard;

	// Food drain per second when owner is using extra energy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food Component")
	double FoodDrainExtra;

public:	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWidgetUpdate OnWidgetUpdate;
	
	UFUNCTION(BlueprintCallable, Category = "Food Component")
	void AdjustFood(const int32 AmountToAdjust);

	UFUNCTION(BlueprintCallable, Category = "Food Component")
	void UpdateMaxFood(const int32 AmountToAdjust);
	
	UFUNCTION()
	void SprintStatusChanged(bool NewStatusIn);
private:
	UPROPERTY(Replicated)
	bool bUsingExtraEnergy;

	UFUNCTION()
	void DealWithFoodConsumed(FName ItemID, const UDataTable* DataTableIn);

	//  Server functions
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SprintStatusChanged(bool NewStatusIn);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateMaxFood(const int32 AmountToAdjust);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AdjustFood(const int32 AmountToAdjust);
	// End of server functions
	
	double FoodAsDouble;

	FTimerHandle FoodDrainTimer;
	float TimerDelay = 0.1f;
	void DrainFood();
};


