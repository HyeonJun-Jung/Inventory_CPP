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
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
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

	UFUNCTION(Server, reliable)
	void Transfer_Slot_Server(UInventoryComponent* SourceInv, int SourceIdx, UInventoryComponent* DestInv, int DestIdx);
	void Transfer_Slot_Server_Implementation(UInventoryComponent* SourceInv, int SourceIdx, UInventoryComponent* DestInv, int DestIdx);

	UFUNCTION(client, Reliable)
	void ShowChestInventory_Client(class UInventoryComponent* ChestInventoryComp);
	void ShowChestInventory_Client_Implementation(class UInventoryComponent* ChestInventoryComp);

	UFUNCTION(client, Reliable)
	void UpdateInventory_Client();
	void UpdateInventory_Client_Implementation();

	UFUNCTION()
	void InventoryUpdate();

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UInventoryComponent* InventoryComponent;

	// For Client Replication Delay
	// Could It be replaced by RepNotify?
	bool ShouldUpdateInventory = false;
	float TimeCountForUpdate = 0.f;
	
};
