// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Interactable_Actor.h"
#include "Interface/Interact_Interface.h"
#include "Chest.generated.h"

UCLASS()
class INVENTORY_CPP_API AChest : public AInteractable_Actor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	virtual void Interact_With_Implementation(UInventoryComponent* InventoryComponent) override;

	UPROPERTY(Replicated)
	UInventoryComponent* Chest_InventoryComponent;
};
