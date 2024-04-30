// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTUStaminaComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float DefaultWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement Speed", meta = (AllowPrivateAccess = "true"))
	float DefaultRunSpeed;
public:	
	// Sets default values for this component's properties
	URTUStaminaComponent();

	void StartSprint();
	void StopSprint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bIsSprinting;

	UPROPERTY()
	AActor* OwningActor;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponentRef;
};
