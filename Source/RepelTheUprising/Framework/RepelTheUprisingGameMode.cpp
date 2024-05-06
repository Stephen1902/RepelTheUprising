// Copyright Epic Games, Inc. All Rights Reserved.

#include "RepelTheUprisingGameMode.h"
#include "../Player/RepelTheUprisingCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

ARepelTheUprisingGameMode::ARepelTheUprisingGameMode()
	: Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PVPMode = EPVPMode::PVPNone;

	Seconds = 0;
	Minutes = 30;
	Hours = 6;
	
	Day = 30;
	Month = 11;
	Year = 2;

	TimeUnit = 2.f;
	Clockwork = 0.f;
	DayTick = 0;	
}

void ARepelTheUprisingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetDayTickAndClockwork(DeltaTime);
	SetClock();
	SetDate();
}

EPVPMode ARepelTheUprisingGameMode::GetPVPMode() const
{
	return PVPMode;
}

void ARepelTheUprisingGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GameTime.Add(Seconds);
	GameTime.Add(Minutes);
	GameTime.Add(Hours);

	GameDate.Add(Day);
	GameDate.Add(Month);
	GameDate.Add(Year);

	Clockwork = Hours + (Minutes / 60.f);
}

void ARepelTheUprisingGameMode::SetDayTickAndClockwork(float DeltaTime)
{
	const float TimeUnitThisFrame = DeltaTime / TimeUnit;
	const float ClockworkThisFrame = TimeUnitThisFrame + Clockwork;
	DayTick = UKismetMathLibrary::FMod(ClockworkThisFrame, 24, Clockwork);
}

void ARepelTheUprisingGameMode::SetClock()
{
	// There are 3600 seconds in an hour 
	const double ClockworkPerHour = Clockwork * 3600;
	double SecondsAsDouble;
	// 60 Seconds in a Minutes
	const int32 RemainingSeconds = UKismetMathLibrary::FMod(ClockworkPerHour, 60, SecondsAsDouble);
	Seconds = FMath::Floor(SecondsAsDouble);
	double MinutesAsDouble;
	// 60 Minutes in an hour
	const int32 RemainingMinutes = UKismetMathLibrary::FMod(RemainingSeconds, 60, MinutesAsDouble);
	Minutes = FMath::Floor(MinutesAsDouble);
	double HoursAsDouble;
	// 24 Hours in a day
	int32 UnneededInt = UKismetMathLibrary::FMod(RemainingMinutes, 24, HoursAsDouble);
	Hours = FMath::Floor(HoursAsDouble);

	if (GameTime.Num() > 0)
	{
		GameTime[0] = Seconds;
		GameTime[1] = Minutes;
		GameTime[2] = Hours;
	}
	else
	{
		GameTime.Add(Seconds);
		GameTime.Add(Minutes);
		GameTime.Add(Hours);
	}

	const FString TimeToDisplay = FString::FromInt(Hours) + ":" + FString::FromInt(Minutes) + ":" + FString::FromInt(Seconds);
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, *TimeToDisplay);
}

void ARepelTheUprisingGameMode::SetDate()
{
	Day += DayTick;
	// Check whether the day is greater than the available days in this month
	if (Day > UKismetMathLibrary::DaysInMonth(Year, Month))
	{
		Day = 1;
		if (Month < 12)
		{
			Month += 1;
		}
		else
		{
			Month = 1;
			Year += 1;
		}
	}

	GameDate[0] = Day;
	GameDate[1] = Month;
	GameDate[2] = Year;
	
	const FString DateToDisplay = FString::FromInt(Day) + "/" + FString::FromInt(Month) + "/" + FString::FromInt(Year);
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, *DateToDisplay);
}
