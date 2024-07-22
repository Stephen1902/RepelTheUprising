// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RTUCraftingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUCraftingComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTUCraftingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// The number of crafting slots the owner of this has
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting", meta=(ClampMin = 1))
	int32 CraftingSlots;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<class URTUInventoryComponent> OwnerInventoryComp;
};
