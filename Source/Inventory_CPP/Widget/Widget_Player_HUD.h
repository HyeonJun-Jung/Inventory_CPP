// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Widget_Player_HUD.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_Player_HUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void ShowInventory(class UInventoryComponent* InventoryComponent);
	void ShowChestInventory(class UInventoryComponent* ChestInvComp, class UInventoryComponent* playerInvComp);
	void UpdateInventory();
	void UpdateCurrentQuickslot(FKey key);

	UClass* InventoryWidgetClass;
	UClass* ChestWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UWidget_QuickSlot_Grid* Widget_QuickSlot_Grid;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY()
	class UWidget_Player_Inventory* Widget_PlayerInventory;

	UPROPERTY()
	class UWidget_Chest_Inventory* Widget_ChestInventory;

};
