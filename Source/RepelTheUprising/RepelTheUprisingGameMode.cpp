// Copyright Epic Games, Inc. All Rights Reserved.

#include "RepelTheUprisingGameMode.h"
#include "RepelTheUprisingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARepelTheUprisingGameMode::ARepelTheUprisingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
