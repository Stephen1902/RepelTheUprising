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

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	EPVPMode GetPVPMode() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time")
	int32 Minutes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time")
	int32 Hours;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time", meta=(ClampMin=1, ClampMax=31))
	int32 Day;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time", meta=(ClampMin=1, ClampMax=12))
	int32 Month;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time", meta=(ClampMin=0))
	int32 Year;

	// Speed at which game time passes in the real world.  1500 is one real hour.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time")
	double TimeUnit;
private:
	UPROPERTY()
	EPVPMode PVPMode;
	
	double Clockwork;
	int32 Seconds;
	int32 DayTick;
	TArray<int32> GameTime;
	TArray<int32> GameDate;

	void SetDayTickAndClockwork(float DeltaTime);
	void SetClock();
	void SetDate();
	
};



