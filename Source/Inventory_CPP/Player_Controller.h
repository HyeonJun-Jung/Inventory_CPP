// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player_Controller.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API APlayer_Controller : public APlayerController
{
	GENERATED_BODY()

public:
	APlayer_Controller();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
		

public:
	class UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }


/*
//////////////////
	HUD Widget
//////////////////
*/
	class UWidget_Player_HUD* HUD;

/*
//////////////////
	Inventory
//////////////////
*/

public:
	void ShowInventory();
	void ShowChestInventory(class UInventoryComponent* ChestInventoryComp);
	void UpdateInventory();

private:
	class UInventoryComponent* InventoryComponent;
	
};
