// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SlotStructure.generated.h"

USTRUCT(BlueprintType)
struct INVENTORY_CPP_API FSlotStructure
{
	GENERATED_BODY()
	FSlotStructure() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Quantity;
};
