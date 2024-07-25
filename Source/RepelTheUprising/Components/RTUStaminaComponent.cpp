// Copyright 2024 DME Games

#include "RTUStaminaComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
URTUStaminaComponent::URTUStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsSprinting = false;
	DefaultWalkSpeed = 300.f;
	DefaultRunSpeed = 900.f;
	DefaultStamina = 100.f;
	CurrentStamina = DefaultStamina;
	MaxStamina = DefaultStamina;
	StaminaDrainPerSecond = 1.0f;
	StaminaRecoveryPerSecond = 1.0f;
	DelayBetweenStaminaRecovery = 1.0f;
	
	OwningActor = nullptr;
	MovementComponentRef = nullptr;
}

void URTUStaminaComponent::StartSprint()
{
	SetCharacterSpeed(DefaultRunSpeed);
	bIsSprinting = true;
	OnSprintingChanged.Broadcast(bIsSprinting);
}

void URTUStaminaComponent::StopSprint()
{
	SetCharacterSpeed(DefaultWalkSpeed);
	bIsSprinting = false;
	OnSprintingChanged.Broadcast(bIsSprinting);
}


void URTUStaminaComponent::AdjustStamina(const float AmountToAdjust)
{
	if (AmountToAdjust != 0.f)
	{
		CurrentStamina += FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
	}
}

// Called when the game starts
void URTUStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningActor = GetOwner();
	MovementComponentRef = Cast<UCharacterMovementComponent>(OwningActor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (MovementComponentRef)
	{
		 SetCharacterSpeed(DefaultWalkSpeed);
	}

	CurrentStamina = DefaultStamina;
	MaxStamina = DefaultStamina;
}

void URTUStaminaComponent::Server_SetCharacterSpeed_Implementation(const float NewCharacterSpeed)
{
	SetCharacterSpeed(NewCharacterSpeed);
}

bool URTUStaminaComponent::Server_SetCharacterSpeed_Validate(const float NewCharacterSpeed)
{
	return MovementComponentRef != nullptr;
}

// Called every frame
void URTUStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSprinting)
	{
		DrainStamina(DeltaTime);
	}
	else
	{
		if (CurrentStamina < MaxStamina)
		{
			RecoverStamina(DeltaTime);
		}
	}
}

void URTUStaminaComponent::SetCharacterSpeed(const float NewCharacterSpeed)
{
	if (!OwningActor->HasAuthority())
	{
		Server_SetCharacterSpeed(NewCharacterSpeed);
		return;
	}

	MovementComponentRef->MaxWalkSpeed = NewCharacterSpeed;
}

void URTUStaminaComponent::DrainStamina(const float DeltaTime)
{
	CurrentStamina -= StaminaDrainPerSecond * DeltaTime;
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	if (CurrentStamina <= 0.f)
	{
		CurrentStamina = 0.f;
		StopSprint();
	}
}

void URTUStaminaComponent::RecoverStamina(const float DeltaTime)
{
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRecoveryPerSecond * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}
}

