// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RTUItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPELTHEUPRISING_API URTUItemComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTUItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Table to use to get this item information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Component")
	FDataTableRowHandle ItemID;

	// Quantity this item holds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Component")
	int32 Quantity;

	virtual void InteractWith_Implementation(class ARepelTheUprisingCharacter* CharacterWhoInteracted) override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FDataTableRowHandle GetItemID() const { return ItemID; }
	int32 GetQuantity() const { return Quantity; }

	void SetItemInfo(const FName INItemID, const int32 InQuantity);
};
