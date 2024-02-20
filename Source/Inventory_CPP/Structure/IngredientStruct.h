// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IngredientStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct INVENTORY_CPP_API FIngredientStruct
{
	GENERATED_BODY()
	FIngredientStruct() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Quantity;
};
