// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory_CPPGameMode.h"
#include "Inventory_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventory_CPPGameMode::AInventory_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
