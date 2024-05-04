// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RTUPlayerState.generated.h"

/**
 * Player state, keeping track of the player levels and upgraded skill tree items
 */
UCLASS()
class REPELTHEUPRISING_API ARTUPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// Sets default settings for this class
	ARTUPlayerState();

	UFUNCTION(BlueprintCallable, Category = "TESTING")
	int32 GetPlayerLevel() const { return PlayerLevel; }

protected:
	// Called on game start
	virtual void BeginPlay() override;

private:
	int32 PlayerLevel;
	int32 ArcheryLevel;
	int32 HandgunLevel;
	int32 ShotgunLevel;
	int32 RifleLevel;
	int32 HandToHandLevel;
	int32 FitnessLevel;
	int32 BarteringLevel;
	int32 MiningLevel;
	int32 BuildingLevel;
	int32 ScavengingLevel;
	int32 ScrappingLevel;

};