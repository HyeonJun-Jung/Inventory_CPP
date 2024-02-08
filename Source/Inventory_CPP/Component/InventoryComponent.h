// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/Item/Iteractable_Item.h"
#include "Structure/SlotStructure.h"
#include "Structure/ItemStructure.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_CPP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Determine Using Tracing or Not
	void Set_Traceable(bool Tracing);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	void Interact_Trace();

	void Interact();

	UFUNCTION(Server, reliable)
	void Interact_Server();
	void Interact_Server_Implementation();


public:
	UPROPERTY(EditAnywhere)
	float Interact_Range = 500;

	UPROPERTY(EditAnywhere)
	float Interact_Radius = 50;

	UPROPERTY(EditAnywhere)
	bool Enable_Tracing = false;

private:
	IInteract_Interface* Interacting_Actor = nullptr;


public:
	UDataTable* GetItemDB() { return ItemDB; };
	TArray<FSlotStructure>& GetContents() { return Contents; }
	TArray<FSlotStructure>& GetQuickSlots() { return QuickSlots; }
	void LogInventoryContents();
	void AddToInventory(FSlotStructure ItemSlot);

	UFUNCTION(Server, reliable, BlueprintCallable)
	void RemoveItem_QuickSlot_Server(int SlotIndex, bool Consumed, int Quantity);
	void RemoveItem_QuickSlot_Server_Implementation(int SlotIndex, bool Consumed, int Quantity);
	
	UFUNCTION(Server, reliable, BlueprintCallable)
	void RemoveItem_Server(int SlotIndex, bool Consumed, int Quantity);
	void RemoveItem_Server_Implementation(int SlotIndex, bool Consumed, int Quantity);

	UFUNCTION(Server, reliable)
	void Transfer_Slot_Server(UInventoryComponent* SourceInv, int SourceIdx, int DestIdx);
	void Transfer_Slot_Server_Implementation(UInventoryComponent* SourceInv, int SourceIdx, int DestIdx);
	
	UFUNCTION(NetMulticast, reliable)
	void UpdateInventory_Multicast();
	void UpdateInventory_Multicast_Implementation();

	void UpdateCurrentQuickSlot(FKey key);


private:
	UPROPERTY()
	UDataTable* ItemDB;

	UPROPERTY(EditAnywhere)
	int Inventory_Size = 30;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	uint8 CurrentQuickSlotNum = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<FSlotStructure> QuickSlots;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<FSlotStructure> Contents;

// Inventory Widget

};
