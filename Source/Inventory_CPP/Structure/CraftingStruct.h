// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IngredientStruct.h"
#include "CraftingStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct INVENTORY_CPP_API FCraftingStruct : public FTableRowBase
{
	GENERATED_BODY()
	FCraftingStruct() {}

	// Item Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredientStruct> Ingredients;
};