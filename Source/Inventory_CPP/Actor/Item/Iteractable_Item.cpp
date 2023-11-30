// Fill out your copyright notice in the Description page of Project Settings.


#include "Iteractable_Item.h"
#include "Structure/ItemStructure.h"
#include "Component/InventoryComponent.h"

// Sets default values
AInteractable_Item::AInteractable_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractable_Item::BeginPlay()
{
	Super::BeginPlay();

	UDataTable* BP_ItemDB = LoadObject<UDataTable>(this, TEXT("/Game/Inventory/Data/DB_ItemData.DB_ItemData"));
	if (IsValid(BP_ItemDB))
	{
		ItemDB = BP_ItemDB;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AInteractable_Item : Can't Get DataTable."))
	}
	
	InitItemData();
}

// Called every frame
void AInteractable_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AInteractable_Item::InitItemData()
{
	if (IsValid(ItemDB))
	{
		FItemStructure* itemData = ItemDB->FindRow<FItemStructure>(Item_ID, Item_ID.ToString());
		if (itemData)
		{
			ItemData_Slot.ID = itemData->ID;
			ItemData_Slot.Quantity = Item_Quantity;
		}
	}
}


void AInteractable_Item::Interact_With_Implementation(UInventoryComponent* InventoryComponent)
{
	UE_LOG(LogTemp, Display, TEXT("Interact_With_Implementation HAS BEEN CALLED"));

	if (InventoryComponent)
	{
		InventoryComponent->AddToInventory(ItemData_Slot);
		InventoryComponent->LogInventoryContents();
		Destroy();
	}
}

void AInteractable_Item::InitItemData_Server_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("InitItemData_Server_Implementation HAS BEEN CALLED"));

	if (IsValid(ItemDB))
	{
		FItemStructure* itemData = ItemDB->FindRow<FItemStructure>(Item_ID, Item_ID.ToString());
		if (itemData)
		{
			ItemData_Slot.ID = itemData->ID;
			ItemData_Slot.Quantity = Item_Quantity;
		}
	}
}

