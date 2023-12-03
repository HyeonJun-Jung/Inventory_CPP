// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interact_Interface.h"
#include "Actor/Interactable_Actor.h"
#include "Structure/SlotStructure.h"
#include "Iteractable_Item.generated.h"

UCLASS()
class INVENTORY_CPP_API AInteractable_Item : public AInteractable_Actor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable_Item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void Interact_With_Implementation(UInventoryComponent* InventoryComponent) override;

	void InitItemData();

	UFUNCTION(Server, reliable)
	void InitItemData_Server();

	void InitItemData_Server_Implementation();

public:
	UPROPERTY()
	UDataTable* ItemDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Item_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Item_Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Item_Quantity;

	UPROPERTY()
	FSlotStructure ItemData_Slot;
};
