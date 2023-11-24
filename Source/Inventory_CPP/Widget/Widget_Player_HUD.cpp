// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Player_HUD.h"
#include "Component/InventoryComponent.h"
#include "Widget_Inventory.h"
#include "Widget/Widget_Player_Inventory.h"

bool UWidget_Player_HUD::Initialize()
{
	Super::Initialize();

	FStringClassReference InventoryWidgetClassRef(TEXT("/Game/Inventory/Widget/WBP_Player_Inventory.WBP_Player_Inventory_C"));
	InventoryWidgetClass = InventoryWidgetClassRef.TryLoadClass<UWidget_Player_Inventory>();
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Load UWidget_Player_Inventory Class."));
		return false;
	}

	Widget_PlayerInventory = CreateWidget<UWidget_Player_Inventory>(this, InventoryWidgetClass);
	if (!Widget_PlayerInventory)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Create UWidget_Player_Inventory Widget."));
		return false;
	}

	if (!CanvasPanel)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Bind Canvas Panel."));
		return false;
	}

	Widget_PlayerInventory->AddToViewport();
	Widget_PlayerInventory->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UWidget_Player_HUD::ShowInventory(UInventoryComponent* InventoryComponent)
{
	if (!Widget_PlayerInventory)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Get UWidget_Player_Inventory Widget."));
		return;
	}

	Widget_PlayerInventory->ShowInventory(InventoryComponent);
}

void UWidget_Player_HUD::UpdateInventory(UInventoryComponent* InventoryComponent)
{
	if(!Widget_PlayerInventory)
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Get UWidget_Player_Inventory Widget."));
	Widget_PlayerInventory->UpdateInventory(InventoryComponent);
}