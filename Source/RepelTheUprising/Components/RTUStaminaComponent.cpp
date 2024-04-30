// Copyright 2024 DME Games

#include "RTUStaminaComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
URTUStaminaComponent::URTUStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsSprinting = false;
	DefaultWalkSpeed = 600.f;
	DefaultRunSpeed = 900.f;
	OwningActor = nullptr;
	MovementComponentRef = nullptr;
}

void URTUStaminaComponent::StartSprint()
{
	bIsSprinting = true;
}

void URTUStaminaComponent::StopSprint()
{
	bIsSprinting = false;
}


// Called when the game starts
void URTUStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningActor = GetOwner();
	MovementComponentRef = Cast<UCharacterMovementComponent>(OwningActor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (MovementComponentRef)
	{
		 MovementComponentRef->MaxWalkSpeed = DefaultWalkSpeed;
	}
}


// Called every frame
void URTUStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSprinting)
	{
		
	}
}

