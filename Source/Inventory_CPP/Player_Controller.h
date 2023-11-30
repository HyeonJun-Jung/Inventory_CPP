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

	UFUNCTION(NetMulticast, reliable)
	void UpdateInventory_Multicast(UInventoryComponent* SourceInv, UInventoryComponent* DestInv);
	void UpdateInventory_Multicast_Implementation(UInventoryComponent* SourceInv, UInventoryComponent* DestInv);

	UFUNCTION(client, Reliable)
	void ShowChestInventory_Client(class UInventoryComponent* ChestInventoryComp);
	void ShowChestInventory_Client_Implementation(class UInventoryComponent* ChestInventoryComp);

	UFUNCTION(client, Reliable)
	void UpdateInventory_Client(UInventoryComponent* SourceInv, UInventoryComponent* DestInv);
	void UpdateInventory_Client_Implementation(UInventoryComponent* SourceInv, UInventoryComponent* DestInv);

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UInventoryComponent* InventoryComponent;
	
};
