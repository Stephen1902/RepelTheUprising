// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTUStaminaComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float DefaultWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float DefaultRunSpeed;

	// Amount of stamina this character starts with
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float DefaultStamina;

	// Amount of time in seconds it takes for 1 unit of stamina to drain when running
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float StaminaDrainPerSecond;

	// Amount of time in seconds it takes for 1 unit of stamina to recover
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryPerSecond;

	// The amount of time, in seconds that stamina takes to recover after it runs out or the running is ended
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float DelayBetweenStaminaRecovery;
public:	
	// Sets default values for this component's properties
	URTUStaminaComponent();

	void StartSprint();
	void StopSprint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server, WithValidation, Reliable, Category = "Functions")
	void Server_SetCharacterSpeed(const float NewCharacterSpeed);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Runs on all
	void SetCharacterSpeed(const float NewCharacterSpeed);

	void DrainStamina(float DeltaTime);
	void RecoverStamina(float DeltaTime);
	
	bool bIsSprinting;
	float CurrentStamina;
	float MaxStamina;

	UPROPERTY()
	AActor* OwningActor;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponentRef;
};
