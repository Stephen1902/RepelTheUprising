// Copyright 2024 DME Games

#include "RTUHealthComponent.h"
#include "..\Framework\RepelTheUprisingGameMode.h"
#include "..\Player\RepelTheUprisingCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URTUHealthComponent::URTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	StartingHealth = 100.f;
	HealthRecovery = 0.0167f;  // Recovers one health per rl minute
	bIsDead = false;

	TeamNum = 255;
	
	SetIsReplicatedByDefault(true);
}


void URTUHealthComponent::AdjustHealth(const double AmountToAdjust)
{
	UE_LOG(LogTemp, Warning, TEXT("Adjust health called.  Amount to add %f"), AmountToAdjust);
	if (AmountToAdjust != 0.f)
	{
		Health = FMath::Clamp(Health + AmountToAdjust, 0.f, MaxHealth);
	}
	UE_LOG(LogTemp, Warning, TEXT("New Health is %f"), Health);
}

void URTUHealthComponent::AdjustMaxHealth(const double AmountToAdjust)
{
	MaxHealth += AmountToAdjust;
}

// Called when the game starts
void URTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// We only want the damage event to fire on the server
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (AActor* MyOwner = GetOwner())
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &URTUHealthComponent::HandleTakeAnyDamage);
		}
	}

	GameModeRef = Cast<ARepelTheUprisingGameMode>(GetWorld()->GetAuthGameMode());
	
	Health = StartingHealth;
	MaxHealth = Health;
}

void URTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HealthRecovery > 0. && !bIsDead && Health < MaxHealth)
	{
		Health = (FMath::Clamp(Health + (HealthRecovery * DeltaTime), 0.f, MaxHealth));

		// OnRep functions aren't called on the server automatically.  Call manually.
		if (GetOwner()->HasAuthority())
		{
			OnRep_Health(0.f);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s has health of %f"), *GetOwner()->GetName(), Health);
}


void URTUHealthComponent::OnRep_Health(double OldHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Health Called"));
	const double Damage = Health - OldHealth;
	
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void URTUHealthComponent::OnRep_MaxHealth()
{
	OnMaxHealthChanged.Broadcast(MaxHealth);
}


void URTUHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Check that there is damage and that the actor isn't considered dead
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	// Check that the owner hasn't damaged themselves and that the actor who damaged this can damage it ie, if allies
	if (DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser))
	{
		return;
	}

	// Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, StartingHealth);

	if (GetOwner()->HasAuthority())
	{
		OnRep_Health(Damage);
	}

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		if (GameModeRef)
		{
			//GameModeRef->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}


void URTUHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));

	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}


bool URTUHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	// Check if the actors are valid	
	if (ActorA == nullptr || ActorB == nullptr)
	{
		// Assume Friendly
		return true;
	}

	// Get the health components of both actors and check they are valid
	const URTUHealthComponent* HealthCompA = Cast<URTUHealthComponent>(ActorA->GetComponentByClass(URTUHealthComponent::StaticClass()));
	const URTUHealthComponent* HealthCompB = Cast<URTUHealthComponent>(ActorB->GetComponentByClass(URTUHealthComponent::StaticClass()));

	if (HealthCompA == nullptr || HealthCompB == nullptr)
	{
		// Assume Enemy
		return false;
	}

	// Get the game mode and check whether the actors can damage each other
	if (GameModeRef)
	{
		const ARepelTheUprisingCharacter* CharA = Cast<ARepelTheUprisingCharacter>(ActorA);
		const ARepelTheUprisingCharacter* CharB = Cast<ARepelTheUprisingCharacter>(ActorB);

		switch (GameModeRef->GetPVPMode())
		{
			case EPVPMode::PVPAll:
				return false;
			case EPVPMode::PVPAllies:
				return HealthCompA->TeamNum == HealthCompB->TeamNum;
			case EPVPMode::PVPStrangers:
				return HealthCompA->TeamNum != HealthCompB->TeamNum;
			case EPVPMode::PVPNone:
				// Check if both damaged actor and damage causer are players
				if (CharA == nullptr || CharB == nullptr)
				{
					// At least one of these is not a player, damage can be caused
					return false;
				}

				return true;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Failed to get valid PVP Mode from Game Mode"));
				//  Assume true
				return true;
		}
		
	}

	// Everything else has failed.  Assume false.
	return false;
}


double URTUHealthComponent::GetHealth() const
{
	return Health;
}

double URTUHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool URTUHealthComponent::GetIsDead() const
{
	return bIsDead;
}


void URTUHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUHealthComponent, Health);
	DOREPLIFETIME(URTUHealthComponent, MaxHealth);
}