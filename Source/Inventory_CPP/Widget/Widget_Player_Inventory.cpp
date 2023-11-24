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

	InventoryWidget = CreateWidget<UWidget_Inventory>(this, InventoryGridClass);
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_Inventory: Can't Create UWidget_Inventory Widget."));
		return false;
	}

	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

    return true;
}

void UWidget_Player_Inventory::ShowInventory(UInventoryComponent* InventoryComponent)
{
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("UWidget_Player_Inventory: Can't Get UWidget_Inventory Widget."));
		return;
	}

	ESlateVisibility InvVisibility = InventoryWidget->GetVisibility();

	if (InvVisibility == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		UWorld* world = GetWorld();
		if (!world) return;
		APlayerController* playerController = world->GetFirstPlayerController();

		FInputModeGameOnly inputMode;
		playerController->SetInputMode(inputMode);
		playerController->bShowMouseCursor = false;
	}
	else if (InvVisibility == ESlateVisibility::Hidden)
	{
		InventoryWidget->UpdateInventoryGrid(InventoryComponent);
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		UWorld* world = GetWorld();
		if (!world) return;
		APlayerController* playerController = world->GetFirstPlayerController();

		FInputModeGameAndUI inputMode;
		inputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		playerController->SetInputMode(inputMode);
		playerController->bShowMouseCursor = true;
	}
}

void UWidget_Player_Inventory::UpdateInventory(UInventoryComponent* InventoryComponent)
{
	InventoryWidget->UpdateInventoryGrid(InventoryComponent);
}
