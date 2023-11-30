// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Chest_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_Chest_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void Show_ChestInventory(class UInventoryComponent* Chest_Inventory, class UInventoryComponent* Player_Inventory);
	void Update_ChestInventory(class UInventoryComponent* SourceInv, class UInventoryComponent* DestInv);

public:
	bool Initialized = false;

	class UInventoryComponent* Player_Inv_Comp;
	class UInventoryComponent* Chest_Inv_Comp;

	UPROPERTY(meta = (BindWidget))
	class UWidget_Inventory* Player_Inventory_Grid;

	UPROPERTY(meta = (BindWidget))
	class UWidget_Inventory* Chest_Inventory_Grid;
};
