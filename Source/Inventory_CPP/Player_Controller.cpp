// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Controller.h"
#include "Component/InventoryComponent.h"
#include "Widget/Widget_Player_HUD.h"
APlayer_Controller::APlayer_Controller()
{
	// Create a Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void APlayer_Controller::BeginPlay()
{
	Super::BeginPlay();

	InventoryComponent->Set_Traceable(true);

	FStringClassReference HUDWidgetClassRef(TEXT("/Game/Inventory/Widget/WBP_Player_HUD.WBP_Player_HUD_C"));
	UClass* HUDWidgetClass = HUDWidgetClassRef.TryLoadClass<UWidget_Player_HUD>();
	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Display, TEXT("APlayer_Controller: Can't Load UWidget_Player_HUD Class."));
		return;
	}

	HUD = CreateWidget<UWidget_Player_HUD>(this, HUDWidgetClass);
	if (!HUD)
	{
		UE_LOG(LogTemp, Display, TEXT("APlayer_Controller: Can't Create UWidget_Player_HUD Widget."));
		return;
	}

	HUD->AddToViewport();
}

void APlayer_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer_Controller::ShowInventory()
{
	if(!InventoryComponent || !HUD) return;
	HUD->ShowInventory(InventoryComponent);
}

void APlayer_Controller::ShowChestInventory(UInventoryComponent* ChestInventoryComp)
{
	if (!InventoryComponent || !HUD) return;
	HUD->ShowChestInventory(ChestInventoryComp, InventoryComponent);
}

void APlayer_Controller::UpdateInventory()
{
	if (!HUD) return;
	HUD->UpdateInventory();
}