// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Player_HUD.h"
#include "Widget/Widget_Inventory.h"
#include "Widget/Widget_Player_Inventory.h"
#include "Widget/Widget_Chest_Inventory.h"
#include "Widget/Widget_QuickSlot_Grid.h"
#include "Component/InventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Player_Controller.h"

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

	UWorld* world = GetWorld();
	if (!IsValid(world)) return false;

	APlayerController* Controller = world->GetFirstPlayerController(); if (!IsValid(Controller)) return false;
	APlayer_Controller* playerController = Cast<APlayer_Controller>(Controller); if (!IsValid(playerController)) return false;
	UInventoryComponent* InventoryComponent = playerController->GetInventoryComponent(); if (!IsValid(InventoryComponent)) return false;

	Widget_QuickSlot_Grid->Initialize_QuickSlotGrid(InventoryComponent);

	return true;
}

void UWidget_Player_HUD::ShowInventory(UInventoryComponent* InventoryComponent)
{
	UWorld* world = GetWorld();
	if (!IsValid(world)) return;
	APlayerController* playerController = world->GetFirstPlayerController();

	if (!IsValid(playerController)) return;

	if (!IsValid(Widget_PlayerInventory))
	{
		Widget_PlayerInventory = CreateWidget<UWidget_Player_Inventory>(playerController, InventoryWidgetClass);
		if (!IsValid(Widget_PlayerInventory))
		{
			UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Create UWidget_Player_Inventory Widget."));
			return;
		}
		Widget_PlayerInventory->AddToViewport();
		Widget_PlayerInventory->ShowInventory(InventoryComponent);
		// Widget_PlayerInventory->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (IsValid(Widget_PlayerInventory))
	{
		if (Widget_PlayerInventory->IsInViewport())
		{
			Widget_PlayerInventory->RemoveFromParent();
			// Widget_PlayerInventory = nullptr;
			FInputModeGameOnly inputMode;
			playerController->SetInputMode(inputMode);
			playerController->bShowMouseCursor = false;
		}
		else
		{
			Widget_PlayerInventory->AddToViewport();
			Widget_PlayerInventory->ShowInventory(InventoryComponent);

			FInputModeGameAndUI inputMode;
			inputMode.SetWidgetToFocus(Widget_PlayerInventory->TakeWidget());
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			playerController->SetInputMode(inputMode);
			playerController->bShowMouseCursor = true;
		}	
		return;
	}
		
}

void UWidget_Player_HUD::UpdateInventory()
{
	if (IsValid(Widget_PlayerInventory) && Widget_PlayerInventory->IsInViewport())
	{
		Widget_PlayerInventory->UpdateInventory();
	}

	if (IsValid(Widget_ChestInventory) && Widget_ChestInventory->IsInViewport())
	{
		Widget_ChestInventory->Update_ChestInventory();
	}

	if (IsValid(Widget_QuickSlot_Grid))
	{
		Widget_QuickSlot_Grid->UpdateInventoryGrid();
	}
}

void UWidget_Player_HUD::UpdateCurrentQuickslot(FKey key)
{
	if (IsValid(Widget_QuickSlot_Grid))
		Widget_QuickSlot_Grid->UpdateCurrentQuickslot(key);
}

void UWidget_Player_HUD::ShowChestInventory(UInventoryComponent* ChestInvComp, UInventoryComponent* playerInvComp)
{
	UWorld* world = GetWorld();
	if (!world) return;
	APlayerController* playerController = world->GetFirstPlayerController();

	if (!IsValid(playerController)) return;

	if (IsValid(Widget_ChestInventory))
	{
		if (Widget_ChestInventory->IsInViewport())
		{
			Widget_ChestInventory->RemoveFromParent();
			// Widget_PlayerInventory = nullptr;
			FInputModeGameOnly inputMode;
			playerController->SetInputMode(inputMode);
			playerController->bShowMouseCursor = false;
		}
		else
		{
			Widget_ChestInventory->AddToViewport();
			Widget_ChestInventory->Show_ChestInventory(ChestInvComp, playerInvComp);

			FInputModeGameAndUI inputMode;
			inputMode.SetWidgetToFocus(Widget_ChestInventory->TakeWidget());
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			playerController->SetInputMode(inputMode);
			playerController->bShowMouseCursor = true;
		}

		return;
	}

	if (!IsValid(Widget_ChestInventory))
	{
		Widget_ChestInventory = CreateWidget<UWidget_Chest_Inventory>(playerController, ChestWidgetClass);
		if (!IsValid(Widget_ChestInventory))
		{
			UE_LOG(LogTemp, Display, TEXT("UWidget_Player_HUD: Can't Create UWidget_Chest_Inventory Widget."));
			return;
		}
		Widget_ChestInventory->AddToViewport();
		Widget_ChestInventory->Show_ChestInventory(ChestInvComp, playerInvComp);
	}
}
