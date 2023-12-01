// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Chest_Inventory.h"
#include "Component/InventoryComponent.h"
#include "Widget_Inventory.h"
#include "Net/UnrealNetwork.h"

bool UWidget_Chest_Inventory::Initialize()
{
	Super::Initialize();

	return true;
}


void UWidget_Chest_Inventory::Show_ChestInventory(UInventoryComponent* Chest_Inventory, UInventoryComponent* Player_Inventory)
{
	if (!Initialized)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Chest_Inventory: Initialized."));
		Player_Inv_Comp = Player_Inventory;
		Player_Inventory_Grid->InitalizeInventoryGrid(Player_Inv_Comp);
		Player_Inventory_Grid->SetVisibility(ESlateVisibility::Hidden);

		Chest_Inv_Comp = Chest_Inventory;
		Chest_Inventory_Grid->InitalizeInventoryGrid(Chest_Inv_Comp);
		Chest_Inventory_Grid->SetVisibility(ESlateVisibility::Hidden);

		Initialized = true;
	}

	ESlateVisibility InvVisibility = Chest_Inventory_Grid->GetVisibility();

	if (InvVisibility == ESlateVisibility::Visible)
	{
		Player_Inventory_Grid->SetVisibility(ESlateVisibility::Hidden);
		Chest_Inventory_Grid->SetVisibility(ESlateVisibility::Hidden);

		UWorld* world = GetWorld();
		if (!world) return;
		APlayerController* playerController = world->GetFirstPlayerController();

		FInputModeGameOnly inputMode;
		playerController->SetInputMode(inputMode);
		playerController->bShowMouseCursor = false;
	}
	else if (InvVisibility == ESlateVisibility::Hidden)
	{
		Player_Inventory_Grid->UpdateInventoryGrid();
		Player_Inventory_Grid->SetVisibility(ESlateVisibility::Visible);

		Chest_Inventory_Grid->UpdateInventoryGrid();
		Chest_Inventory_Grid->SetVisibility(ESlateVisibility::Visible);

		UWorld* world = GetWorld();
		if (!world) return;
		APlayerController* playerController = world->GetFirstPlayerController();

		FInputModeGameAndUI inputMode;
		inputMode.SetWidgetToFocus(TakeWidget());
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		playerController->SetInputMode(inputMode);
		playerController->bShowMouseCursor = true;
	}
}

void UWidget_Chest_Inventory::Update_ChestInventory()
{
	Player_Inventory_Grid->UpdateInventoryGrid();
	Chest_Inventory_Grid->UpdateInventoryGrid();
}
