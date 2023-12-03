// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Player_Inventory.h"
#include "Components/Button.h"
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

void UWidget_Player_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(Button_Exit))
		Button_Exit->OnClicked.AddUniqueDynamic(this, &UWidget_Player_Inventory::Exit);
}

void UWidget_Player_Inventory::ShowInventory(UInventoryComponent* InventoryComponent)
{
	if (!Initialized)
	{
		Player_Inventory_Grid->InitalizeInventoryGrid(InventoryComponent);
		Player_Inventory_Grid->SetVisibility(ESlateVisibility::Visible);
		Initialized = true;
	}

	Player_Inventory_Grid->UpdateInventoryGrid();
}

void UWidget_Player_Inventory::UpdateInventory()
{
	Player_Inventory_Grid->UpdateInventoryGrid();
}

void UWidget_Player_Inventory::Exit()
{
	UWorld* world = GetWorld();
	if (!world) return;
	APlayerController* playerController = world->GetFirstPlayerController();

	if (!IsValid(playerController)) return;

	FInputModeGameOnly inputMode;
	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = false;

	RemoveFromParent();
}

