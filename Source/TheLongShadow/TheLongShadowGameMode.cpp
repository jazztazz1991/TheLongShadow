// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheLongShadowGameMode.h"
#include "TheLongShadowCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheLongShadowGameMode::ATheLongShadowGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
