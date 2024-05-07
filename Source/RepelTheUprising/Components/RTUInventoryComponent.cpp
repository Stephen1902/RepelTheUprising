// Copyright 2024 DME Games


#include "RTUInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"

#define InteractTrace ECC_GameTraceChannel2

// Sets default values for this component's properties
URTUInventoryComponent::URTUInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TimeBetweenTraceCheck = 0.1f;
	TimeSinceTraceLastCheck = 0.f;
	InteractionRange = 300.f;
	
	SetIsReplicatedByDefault(true);
}

void URTUInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Check whether what this component is connected to is a player
	PlayerCharacterRef = Cast<ARepelTheUprisingCharacter>(GetOwner());
}

void URTUInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Only the player character needs to do a line trace
	if (PlayerCharacterRef != nullptr)
	{
		TimeSinceTraceLastCheck += DeltaTime;
		if (TimeSinceTraceLastCheck >= TimeBetweenTraceCheck)
		{
			DoInteractTrace();
		}
	}
}

void URTUInventoryComponent::AddToInventory()
{
	
}

void URTUInventoryComponent::RemoveFromInventory()
{
	
}

void URTUInventoryComponent::DoInteractTrace()
{
	TimeSinceTraceLastCheck = 0.f;

	FVector EyesLoc;
	FRotator EyesRot;
	PlayerCharacterRef->GetActorEyesViewPoint(EyesLoc, EyesRot);

	const FVector EndLoc = (EyesRot.Vector() * InteractionRange) + EyesLoc;
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, EyesLoc, EndLoc, InteractTrace, QueryParams);

	if (HitResult.IsValidBlockingHit())
	{
		// Check that the actor we hit is a new one
		if (CurrentlyViewedActor != HitResult.GetActor())
		{
			CurrentlyViewedActor = HitResult.GetActor();
			const FText TextToDisplay = Execute_LookAt(CurrentlyViewedActor);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *TextToDisplay.ToString());
		}
	}
	else if (CurrentlyViewedActor != nullptr)
	{
		// The player isn't looking at an interactive item, clear it
		CurrentlyViewedActor = nullptr;
	}
}

void URTUInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTUInventoryComponent, SlotStruct);
}
