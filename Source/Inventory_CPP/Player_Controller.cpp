// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Controller.h"
#include "Component/InventoryComponent.h"
#include "Widget/Widget_Player_HUD.h"
#include "Net/UnrealNetwork.h"
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

	if (ShouldUpdateInventory)
	{
		TimeCountForUpdate += DeltaTime;
		if (TimeCountForUpdate >= 0.1)
		{
			if (IsValid(HUD))
				HUD->UpdateInventory();
			TimeCountForUpdate = 0.f;
			ShouldUpdateInventory = false;
		}
	}
}

void APlayer_Controller::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayer_Controller, InventoryComponent);
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

void APlayer_Controller::Transfer_Slot_Server_Implementation(UInventoryComponent* SourceInv, int SourceIdx, UInventoryComponent* DestInv, int DestIdx)
{
	// Test
	TArray<FSlotStructure>& SourceContents = SourceInv->GetContents();
	TArray<FSlotStructure>& DestContents = DestInv->GetContents();

	auto ItemDB = SourceInv->GetItemDB();

	// Case : SourceItemID == DestItemID
	if (DestContents[DestIdx].ID == SourceContents[SourceIdx].ID)
	{
		UE_LOG(LogTemp, Warning, TEXT("Local Contests[%d] : %s"), DestIdx, *DestContents[DestIdx].ID.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Source Contests[%d] : %s"), SourceIdx, *SourceContents[DestIdx].ID.ToString());
		FItemStructure* itemData = ItemDB->FindRow<FItemStructure>(DestContents[DestIdx].ID, DestContents[DestIdx].ID.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't find Item ID <%s> From Contents."), *DestContents[DestIdx].ID.ToString());
			return;
		}

		if (SourceInv == DestInv)
			return;

		// Case : Dest ItemQuantity + Source ItemQuantity <= MaxStackSize
		if (DestContents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity <= itemData->MaxStackSize)
		{
			DestContents[DestIdx].Quantity += SourceContents[SourceIdx].Quantity;
			SourceContents[SourceIdx].Quantity = 0;
			SourceContents[SourceIdx].ID = FName("Empty");
		}
		// Case : Dest ItemQuantity + Source ItemQuantity > MaxStackSize
		else
		{
			int TransferQuantity
				= DestContents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity - itemData->MaxStackSize;

			DestContents[DestIdx].Quantity += TransferQuantity;
			SourceContents[SourceIdx].Quantity -= TransferQuantity;
		}
	}
	// Case : SourceItemID != DestItemID
	else
	{
		FSlotStructure LocalSlot = DestContents[DestIdx];
		FSlotStructure SrcSlot = SourceContents[SourceIdx];
		DestContents[DestIdx] = SrcSlot;
		SourceContents[SourceIdx] = LocalSlot;
	}

	DestInv->UpdateInventory_Multicast();
	SourceInv->UpdateInventory_Multicast();

	// UpdateInventory_Multicast_Implementation(SourceInv, DestInv);
}

void APlayer_Controller::ShowChestInventory_Client_Implementation(UInventoryComponent* ChestInventoryComp)
{
	if (!InventoryComponent || !HUD) return;
	HUD->ShowChestInventory(ChestInventoryComp, InventoryComponent);
}

void APlayer_Controller::UpdateInventory_Client_Implementation()
{
	if (!HUD) return;
	ShouldUpdateInventory = true;
	HUD->UpdateInventory();
}

void APlayer_Controller::InventoryUpdate()
{
	if (!HUD) return;
	ShouldUpdateInventory = true;
	HUD->UpdateInventory();
}
