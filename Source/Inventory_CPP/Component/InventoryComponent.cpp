// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Interface/Interact_Interface.h"
#include "Actor/Interactable_Actor.h"
#include "Player_Controller.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UDataTable* BP_ItemDB = LoadObject<UDataTable>(this, TEXT("/Game/Inventory/Data/DB_ItemData.DB_ItemData"));
	if (IsValid(BP_ItemDB))
	{
		ItemDB = BP_ItemDB;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't Get DataTable."))
	}

	
	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStructure item; item.ID = FName("Empty"); item.Quantity = 0;
		Contents.Add(item);
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Enable_Tracing)
		Interact_Trace();
}


void UInventoryComponent::Set_Traceable(bool Tracing)
{
	Enable_Tracing = Tracing;
}

void UInventoryComponent::Interact_Trace()
{
	AController* controller = Cast<AController>(GetOwner());
	if (!controller) return;

	APawn* pawn = controller->GetPawn();
	if (!pawn) return;

	TArray<AActor*> IgnoreActors;	FHitResult hitResult;
	IgnoreActors.Add(pawn);

	FVector start = pawn->GetActorLocation();
	FRotator ControllerRot = controller->GetControlRotation();
	FVector ControllerForwardVec = UKismetMathLibrary::GetForwardVector(ControllerRot);
	// FVector end = start + Interact_Range * owner->GetActorForwardVector();
	FVector end = start + Interact_Range * ControllerForwardVec;

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

	UKismetSystemLibrary::SphereTraceSingle(pawn, start, end, Interact_Radius,
		TraceType, false, IgnoreActors,
		EDrawDebugTrace::ForOneFrame, hitResult, true);

	IInteract_Interface* hitActor = Cast<IInteract_Interface>(hitResult.GetActor());

	if (hitActor)
	{
		if (!Interacting_Actor)
		{
			Interacting_Actor = hitActor;
		}
		else
		{

		}
	}
	else
	{
		if (Interacting_Actor)
		{
			Interacting_Actor = nullptr;
		}
	}
		
}

void UInventoryComponent::Interact()
{
	if (Interacting_Actor)
	{
		AInteractable_Actor* interactable_Actor = Cast<AInteractable_Actor>(Interacting_Actor);
		if (interactable_Actor)
		{
			interactable_Actor->Execute_Interact_With(interactable_Actor, this);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Interacting Actor Is not IInteract_Interface Ref."));
	}
}

void UInventoryComponent::LogInventoryContents()
{
	for (FSlotStructure slot : Contents)
	{
		if(!slot.ID.IsEqual(FName("Empty")))
			UE_LOG(LogTemp, Warning, TEXT("ID : %s, Quantity : %d"), *slot.ID.ToString(), slot.Quantity);
	}
}

void UInventoryComponent::AddToInventory(FSlotStructure ItemSlot)
{
	if (!ItemDB)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't Get DataTable."), *ItemSlot.ID.ToString());
		return;
	}

	FItemStructure* itemData = ItemDB->FindRow<FItemStructure>(ItemSlot.ID, ItemSlot.ID.ToString());
	if (!itemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : ID <%s> Can't Add To Inventory."), *ItemSlot.ID.ToString());
		return;
	}
		
	for (FSlotStructure& slot : Contents)
	{
		if (slot.ID.IsEqual(ItemSlot.ID) && slot.Quantity + ItemSlot.Quantity <= itemData->MaxStackSize)
		{
			slot.Quantity += ItemSlot.Quantity;
			return;
		}
	}

	for (FSlotStructure& slot : Contents)
	{
		if (slot.ID.IsEqual(FName("Empty")))
		{
			slot = ItemSlot;
			return;
		}
	}
}

void UInventoryComponent::Transfer_Slot_Server_Implementation(UInventoryComponent* SourceInv, int SourceIdx, int DestIdx)
{
	TArray<FSlotStructure>& SourceContents = SourceInv->GetContents();

	// Case : SourceItemID == DestItemID
	if (Contents[DestIdx].ID == SourceContents[SourceIdx].ID)
	{
		UE_LOG(LogTemp, Warning, TEXT("Local Contests[%d] : %s"), DestIdx, *Contents[DestIdx].ID.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Source Contests[%d] : %s"), SourceIdx, *SourceContents[DestIdx].ID.ToString());
		FItemStructure* itemData = ItemDB->FindRow<FItemStructure>(Contents[DestIdx].ID, Contents[DestIdx].ID.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't find Item ID <%s> From Contents."), *Contents[DestIdx].ID.ToString());
			return;
		}

		if (SourceInv == this)
			return;

		// Case : Dest ItemQuantity + Source ItemQuantity <= MaxStackSize
		if (Contents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity <= itemData->MaxStackSize)
		{
			Contents[DestIdx].Quantity += SourceContents[SourceIdx].Quantity;
			SourceContents[SourceIdx].Quantity = 0;
			SourceContents[SourceIdx].ID = FName("Empty");
		}
		// Case : Dest ItemQuantity + Source ItemQuantity > MaxStackSize
		else
		{
			int TransferQuantity
				= Contents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity - itemData->MaxStackSize;

			Contents[DestIdx].Quantity += TransferQuantity;
			SourceContents[SourceIdx].Quantity -= TransferQuantity;
		}
	}
	// Case : SourceItemID != DestItemID
	else
	{
		FSlotStructure LocalSlot = Contents[DestIdx];
		FSlotStructure SrcSlot = SourceContents[SourceIdx]; 
		Contents[DestIdx] = SrcSlot;
		SourceContents[SourceIdx] = LocalSlot;
	}

	UpdateInventory_Multicast();
	SourceInv->UpdateInventory_Multicast();
}

void UInventoryComponent::UpdateInventory_Multicast_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("UInventoryComponent : UpdateInventory_Multicast_Implementation HAS BEEN CALLED."));
	APlayer_Controller* controller = Cast< APlayer_Controller>(GetWorld()->GetFirstPlayerController());
	if (!controller)
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't Cast to Player_Controller."));
	controller->UpdateInventory();
}