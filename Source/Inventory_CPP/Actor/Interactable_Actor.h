// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interact_Interface.h"
#include "Interactable_Actor.generated.h"

UCLASS()
class INVENTORY_CPP_API AInteractable_Actor : public AActor, public IInteract_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Interact_With_Implementation(UInventoryComponent* InventoryComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
