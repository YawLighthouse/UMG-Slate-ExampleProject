// Copyright Epic Games, Inc. All Rights Reserved.

#include "NicksExampleProjectGameMode.h"
#include "NicksExampleProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANicksExampleProjectGameMode::ANicksExampleProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
