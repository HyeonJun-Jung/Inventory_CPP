// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Player_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_Player_Inventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	
public:
	virtual void NativeConstruct() override;
	void ShowInventory(class UInventoryComponent* InventoryComponent);
	void UpdateInventory();
	
	UFUNCTION()
	void Exit();

	bool Initialized = false;

	UClass* InventoryGridClass;

	UPROPERTY(meta = (BindWidget))
	class UWidget_Inventory* Player_Inventory_Grid;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Exit;
};
