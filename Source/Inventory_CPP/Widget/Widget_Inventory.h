// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void UpdateInventoryGrid(class UInventoryComponent* InventoryComponent);


public:
	class UInventoryComponent* InvComp;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* row_1;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* row_2;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* row_3;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* row_4;

	TArray<class UWidget_Inventory_Slot*> InventorySlot_Array;
};
