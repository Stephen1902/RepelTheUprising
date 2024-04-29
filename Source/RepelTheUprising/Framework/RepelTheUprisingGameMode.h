// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RepelTheUprisingGameMode.generated.h"

UENUM()
enum class EPVPMode : uint8
{
	PVPAll			UMETA(DisplayName = "Kill All"),
	PVPAllies		UMETA(DisplayName = "Kill Allies Only"),
	PVPStrangers	UMETA(DisplayName = "Kill Strangers Only"),
	PVPNone			UMETA(DisplayName = "No PVP")
};

UCLASS(minimalapi)
class ARepelTheUprisingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARepelTheUprisingGameMode();

	EPVPMode GetPVPMode() const;

private:
	UPROPERTY()
	EPVPMode PVPMode;
};



