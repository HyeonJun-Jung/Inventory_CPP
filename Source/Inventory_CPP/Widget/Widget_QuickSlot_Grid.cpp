// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_QuickSlot_Grid.h"
#include "Components/HorizontalBox.h"
#include "Component/InventoryComponent.h"
#include "Widget/Widget_QuickSlot.h"

bool UWidget_QuickSlot_Grid::Initialize()
{
	Super::Initialize(); 

	return true;
}

void UWidget_QuickSlot_Grid::Initialize_QuickSlotGrid(UInventoryComponent* InventoryComponent)
{
	InvComp = InventoryComponent;
	if (!InvComp)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_QuickSlot_Grid: Can't Get Inventory Component."));
		return;
	}

	if (!IsValid(QuickSlots)) return;
	TArray<UWidget*> QuickSlotArr = QuickSlots->GetAllChildren();

	int slotNum = 0;
	for (UWidget* widget : QuickSlotArr)
	{
		UWidget_QuickSlot* QuickSlot = Cast<UWidget_QuickSlot>(widget);
		if (!QuickSlot) {
			UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: Can't Cast To UWidget_Inventory_Slot."));
			return;
		}
		QuickSlot->InitializeSlot(slotNum++, InvComp);
		QuickSlot_Array.Add(QuickSlot);
	}

	return;
}

void UWidget_QuickSlot_Grid::UpdateInventoryGrid()
{
	if (!InvComp) return;

	auto InvQuickSlots = InvComp->GetQuickSlots(); int idx = 0;
	auto ItemDB = InvComp->GetItemDB();

	for (const FSlotStructure slot : InvQuickSlots)
	{
		if (!slot.ID.IsEqual(FName("Empty")))
			UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: <UpdateInventoryGrid> Item ID  %s."), *slot.ID.ToString());

		auto Item = ItemDB->FindRow<FItemStructure>(slot.ID, slot.ID.ToString());
		if (!Item) {
			continue;
		}

		if (idx < QuickSlot_Array.Num())
		{
			QuickSlot_Array[idx++]->UpdateSlot(*Item, slot.Quantity);
		}
	}
}
