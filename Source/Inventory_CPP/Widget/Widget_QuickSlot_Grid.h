// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_QuickSlot_Grid.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_QuickSlot_Grid : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	virtual void Initialize_QuickSlotGrid(class UInventoryComponent* InventoryComponent);
	void UpdateInventoryGrid();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentQuickslot(FKey key);

	class UInventoryComponent* InvComp;
	
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* QuickSlots;

	TArray<class UWidget_QuickSlot*> QuickSlot_Array;
};
