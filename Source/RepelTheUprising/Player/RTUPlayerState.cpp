// Copyright 2024 DME Games

#include "RTUPlayerState.h"

ARTUPlayerState::ARTUPlayerState()
{
	PlayerLevel = 1;
	ArcheryLevel = 1;
	HandgunLevel = 1;
	ShotgunLevel = 1;
	RifleLevel = 1;
	HandToHandLevel = 1;
	FitnessLevel = 1;
	BarteringLevel = 1;
	MiningLevel = 1;
	BuildingLevel = 1;
	ScavengingLevel = 1;
	ScrappingLevel = 1;

}

void ARTUPlayerState::BeginPlay()
{
	Super::BeginPlay();


}