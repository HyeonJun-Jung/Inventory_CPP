// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Chest/Chest.h"
#include "Player_Controller.h"
#include "Component/InventoryComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Chest_InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("Inventory"));
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChest::Interact_With_Implementation(UInventoryComponent* InventoryComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("AChest : Interact_With_Implementation HAS BEEN CALLED."));

	APlayer_Controller* controller = Cast<APlayer_Controller>(InventoryComponent->GetOwner());
	if (!controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("AChest : Can't Get PlayerController."));
		return;
	}
	
	controller->ShowChestInventory(Chest_InventoryComponent);
}

