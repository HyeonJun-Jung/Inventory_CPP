// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Player_HUD.h"
#include "Widget/Widget_Inventory.h"
#include "Widget/Widget_Player_Inventory.h"
#include "Widget/Widget_Chest_Inventory.h"
#include "Component/InventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"

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

	FStringClassReference ChestWidgetClassRef(TEXT("/Game/Inventory/Widget/WBP_Chest_Inventory.WBP_Chest_Inventory_C"));
	ChestWidgetClass = ChestWidgetClassRef.TryLoadClass<UWidget_Chest_Inventory>();

	if (!CanvasPanel)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Bind Canvas Panel."));
		return false;
	}

	return true;
}

void UWidget_Player_HUD::ShowInventory(UInventoryComponent* InventoryComponent)
{
	if (!IsValid(Widget_PlayerInventory))
	{
		Widget_PlayerInventory = CreateWidget<UWidget_Player_Inventory>(this, InventoryWidgetClass);
		if (!IsValid(Widget_PlayerInventory))
		{
			UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Create UWidget_Player_Inventory Widget."));
			return;
		}
		Widget_PlayerInventory->AddToViewport();
		// Widget_PlayerInventory->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IsValid(Widget_PlayerInventory))
		Widget_PlayerInventory->ShowInventory(InventoryComponent);
}

void UWidget_Player_HUD::UpdateInventory(UInventoryComponent* SourceInv, UInventoryComponent* DestInv)
{
	if (IsValid(Widget_PlayerInventory))
	{
		if(SourceInv == DestInv)
			Widget_PlayerInventory->UpdateInventory();
	}

	if (IsValid(Widget_ChestInventory))
	{
		Widget_ChestInventory->Update_ChestInventory(SourceInv, DestInv);
	}
}

void UWidget_Player_HUD::ShowChestInventory(UInventoryComponent* ChestInvComp, UInventoryComponent* playerInvComp)
{
	if (IsValid(Widget_ChestInventory))
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(Widget_ChestInventory);
		Widget_ChestInventory = nullptr;

		UWorld* world = GetWorld();
		if (!world) return;
		APlayerController* playerController = world->GetFirstPlayerController();

		FInputModeGameOnly inputMode;
		playerController->SetInputMode(inputMode);
		playerController->bShowMouseCursor = false;
		return;
	}

	if (!IsValid(Widget_ChestInventory))
	{
		Widget_ChestInventory = CreateWidget<UWidget_Chest_Inventory>(this, ChestWidgetClass);
		if (!IsValid(Widget_ChestInventory))
		{
			UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Create UWidget_Chest_Inventory Widget."));
			return;
		}
		Widget_ChestInventory->AddToViewport();
		Widget_ChestInventory->Show_ChestInventory(ChestInvComp, playerInvComp);
	}
}
