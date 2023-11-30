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
	void LogInventoryContents();
	void AddToInventory(FSlotStructure ItemSlot);
	
	UFUNCTION(Server, reliable)
	void Transfer_Slot_Server(UInventoryComponent* SourceInv, int SourceIdx, int DestIdx);
	void Transfer_Slot_Server_Implementation(UInventoryComponent* SourceInv, int SourceIdx, int DestIdx);

	UFUNCTION(NetMulticast, reliable)
	void UpdateInventory_Multicast(UInventoryComponent* SourceInv, UInventoryComponent* DestInv);
	void UpdateInventory_Multicast_Implementation(UInventoryComponent* SourceInv, UInventoryComponent* DestInv);


private:
	UPROPERTY()
	UDataTable* ItemDB;

	UPROPERTY(EditAnywhere)
	int Inventory_Size = 30;

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<FSlotStructure> Contents;

// Inventory Widget

};
