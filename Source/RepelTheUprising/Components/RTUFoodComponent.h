// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTUFoodComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFoodChanged, URTUFoodComponent*, OwningFoodComp, float, HealthToChange, float, HealthDelta);

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
	float Food;

	UFUNCTION()
	void OnRep_Food(float OldFood);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food Component")
	float StartingFood;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetFood() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFoodChanged OnFoodChanged;

	UFUNCTION(BlueprintCallable, Category = "Food Component")
	void ConsumeFood(const float FoodAmount);
};
