// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Inventory.h"
#include "Widget/Widget_Inventory_Slot.h"
#include "Player_Controller.h"
#include "Components/HorizontalBox.h"
#include "Component/InventoryComponent.h"

bool UWidget_Inventory::Initialize()
{
	Super::Initialize();
		
	return true;
}

void UWidget_Inventory::InitalizeInventoryGrid(UInventoryComponent* InventoryComponent)
{
	InvComp = InventoryComponent;
	if (!InvComp)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: Can't Get Inventory Component."));
		return;
	}

	if (!row_1) return;
	TArray<UWidget*> Array1 = row_1->GetAllChildren();

	int slotNum = 0;
	for (UWidget* widget : Array1)
	{
		UWidget_Inventory_Slot* InvSlot = Cast<UWidget_Inventory_Slot>(widget);
		if (!InvSlot) {
			UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: Can't Cast To UWidget_Inventory_Slot."));
			return;
		}
		InvSlot->InitializeSlot(slotNum++, InvComp);
		InventorySlot_Array.Add(InvSlot);
	}

	if (!row_2) return;
	TArray<UWidget*> Array2 = row_2->GetAllChildren();

	for (UWidget* widget : Array2)
	{
		UWidget_Inventory_Slot* InvSlot = Cast<UWidget_Inventory_Slot>(widget);
		if (!InvSlot) {
			UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: Can't Cast To UWidget_Inventory_Slot."));
			return;
		}
		InvSlot->InitializeSlot(slotNum++, InvComp);
		InventorySlot_Array.Add(InvSlot);
	}

	if (!row_3) return;
	TArray<UWidget*> Array3 = row_3->GetAllChildren();

	for (UWidget* widget : Array3)
	{
		UWidget_Inventory_Slot* InvSlot = Cast<UWidget_Inventory_Slot>(widget);
		if (!InvSlot) {
			UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: Can't Cast To UWidget_Inventory_Slot."));
			return;
		}
		InvSlot->InitializeSlot(slotNum++, InvComp);
		InventorySlot_Array.Add(InvSlot);
	}

	if (!row_4) return;
	TArray<UWidget*> Array4 = row_4->GetAllChildren();

	for (UWidget* widget : Array4)
	{
		UWidget_Inventory_Slot* InvSlot = Cast<UWidget_Inventory_Slot>(widget);
		if (!InvSlot) {
			UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: Can't Cast To UWidget_Inventory_Slot."));
			return;
		}
		InvSlot->InitializeSlot(slotNum++, InvComp);
		InventorySlot_Array.Add(InvSlot);
	}

}

void UWidget_Inventory::UpdateInventoryGrid()
{
	if (!InvComp) return;

	auto Contents = InvComp->GetContents(); int idx = 0;
	auto ItemDB = InvComp->GetItemDB();

	for (FSlotStructure slot : Contents)
	{
		if(!slot.ID.IsEqual(FName("Empty")))
			UE_LOG(LogTemp, Display, TEXT("UWidget_Inventory: <UpdateInventoryGrid> Item ID  %s."), *slot.ID.ToString());

		auto Item = ItemDB->FindRow<FItemStructure>(slot.ID, slot.ID.ToString());
		if (!Item) {
			continue;
		}

		if (idx < InventorySlot_Array.Num())
		{
			InventorySlot_Array[idx++]->UpdateSlot(Item->Name, Item->Icon, slot.Quantity);
		}
	}
}
