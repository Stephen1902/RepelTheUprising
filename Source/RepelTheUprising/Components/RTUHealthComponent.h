// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTUHealthComponent.generated.h"

// OnHealthChanged event
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, URTUHealthComponent*, OwningHealthComp, double, Health, double, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, double, MaxHealth);

UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTUHealthComponent();

	// Reference to the Team Number of the owner of this component.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	uint8 TeamNum;

	void AdjustHealth(const double AmountToAdjust);
	void AdjustMaxHealth(const double AmountToAdjust);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category = "Health Component")
	double Health;

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category = "Health Component")
	double MaxHealth;

	UFUNCTION()
	void OnRep_Health(double OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth();

	// Health the owner of this component starts with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	double StartingHealth;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
public:

	double GetHealth() const;

	bool GetIsDead() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
	bool IsFriendly(AActor* ActorA, AActor* ActorB);

private:
	UPROPERTY()
	class ARepelTheUprisingGameMode* GameModeRef;
	
	bool bIsDead;
};