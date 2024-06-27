// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP3GameMode.h"
#include "GP3Character.h"
#include "UObject/ConstructorHelpers.h"

AGP3GameMode::AGP3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
