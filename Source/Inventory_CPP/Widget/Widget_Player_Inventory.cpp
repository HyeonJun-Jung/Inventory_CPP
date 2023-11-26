// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Player_Inventory.h"
#include "Component/InventoryComponent.h"
#include "Widget_Inventory.h"

bool UWidget_Player_Inventory::Initialize()
{
    Super::Initialize();

	FStringClassReference InventoryWidgetClassRef(TEXT("/Game/Inventory/Widget/WBP_Inventory_Grid.WBP_Inventory_Grid_C"));
	InventoryGridClass = InventoryWidgetClassRef.TryLoadClass<UWidget_Inventory>();
	if (!InventoryGridClass)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_Inventory: Can't Load UWidget_Inventory Class."));
		return false;
	}

    return true;
}

void UWidget_Player_Inventory::ShowInventory(UInventoryComponent* InventoryComponent)
{
	if (!Initialized)
	{
		Player_Inventory_Grid->InitalizeInventoryGrid(InventoryComponent);
		Player_Inventory_Grid->SetVisibility(ESlateVisibility::Hidden);
		Initialized = true;
	}

	ESlateVisibility InvVisibility = Player_Inventory_Grid->GetVisibility();

	if (InvVisibility == ESlateVisibility::Visible)
	{
		Player_Inventory_Grid->SetVisibility(ESlateVisibility::Hidden);

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

		UWorld* world = GetWorld();
		if (!world) return;
		APlayerController* playerController = world->GetFirstPlayerController();

		FInputModeGameAndUI inputMode;
		inputMode.SetWidgetToFocus(Player_Inventory_Grid->TakeWidget());
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		playerController->SetInputMode(inputMode);
		playerController->bShowMouseCursor = true;
	}
}

void UWidget_Player_Inventory::UpdateInventory()
{
	Player_Inventory_Grid->UpdateInventoryGrid();
}