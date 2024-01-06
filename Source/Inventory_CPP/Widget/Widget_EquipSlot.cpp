// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_EquipSlot.h"
#include "Widget/Widget_Inventory_Slot.h"
#include "Widget/SlotDragDrop.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Player_Controller.h"
#include "Inventory_CPP/Inventory_CPPCharacter.h"
#include "Component/InventoryComponent.h"
#include <Blueprint/WidgetBlueprintLibrary.h>

bool UWidget_EquipSlot::Initialize()
{
	Super::Initialize();
	return true;
}

void UWidget_EquipSlot::InitializeSlot(UInventoryComponent* InventoryComp)
{
	InvComp = InventoryComp;
}

void UWidget_EquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (IsValid(OutOperation))
		UE_LOG(LogTemp, Warning, TEXT("UWidget_EquipSlot : Drag Again."));

	UE_LOG(LogTemp, Warning, TEXT("UWidget_EquipSlot : Drag Detected."));

	USlotDragDrop* DragDropOperation = NewObject<USlotDragDrop>();
	OutOperation = DragDropOperation;
	DragDropOperation->DefaultDragVisual = this;
}

bool UWidget_EquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UWidget_Inventory_Slot* DragVisualWidget = Cast<UWidget_Inventory_Slot>(InOperation->DefaultDragVisual);

	if (!DragVisualWidget->Item_Category.IsEqual(FName("Equipment")))
		return true;

	UInventoryComponent* SourceInv = DragVisualWidget->InvComp;
	int SourceIdx = DragVisualWidget->SlotIdx;
	FName itemName = DragVisualWidget->Item_Name;

	
	
	APlayerController* LocalController = GetGameInstance()->GetFirstLocalPlayerController();
	if (IsValid(LocalController))
	{
		AInventory_CPPCharacter* Character = Cast<AInventory_CPPCharacter>(LocalController->GetCharacter());
		if (IsValid(Character))
		{
			// Equip Item Through Character
			Character->AttachEquipment_Server(DragVisualWidget->Item_ID);

			EquipItem_Image->SetBrushFromTexture(DragVisualWidget->Item_Texture2D);
			EquipItem_Image->SetVisibility(ESlateVisibility::Visible);

			Item_ID = DragVisualWidget->Item_ID;
			Item_Name = DragVisualWidget->Item_Name;
			Item_Texture2D = DragVisualWidget->Item_Texture2D;

			// Remove Item From Inventroy
			SourceInv->RemoveItem_Server(SourceIdx, true, 1);
		}
	}

	//if (IsValid(LocalController))
	//{
	//	APlayer_Controller* controller = Cast<APlayer_Controller>(LocalController);
	//	if (IsValid(controller))
	//	{
	//		// Equip Item Through Controller
	//		controller->AttachEquipment_Server(DragVisualWidget->Item_ID);

	//		EquipItem_Image->SetBrushFromTexture(DragVisualWidget->Item_Texture2D);
	//		EquipItem_Image->SetVisibility(ESlateVisibility::Visible);

	//		Item_ID = DragVisualWidget->Item_ID;
	//		Item_Name = DragVisualWidget->Item_Name;
	//		Item_Texture2D = DragVisualWidget->Item_Texture2D;

	//		// Remove Item From Inventroy
	//		SourceInv->RemoveItem_Server(SourceIdx, true, 1);
	//	}
	//}

	return true;
}

FEventReply UWidget_EquipSlot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if (!Item_Name.IsEqual(FName("None")))
		{
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UWidget_EquipSlot : Item Is None."));
		}
	}

	return Reply;
}

void UWidget_EquipSlot::DetachEquipment()
{
	APlayerController* LocalController = GetGameInstance()->GetFirstLocalPlayerController();
	if (IsValid(LocalController))
	{
		AInventory_CPPCharacter* Character = Cast<AInventory_CPPCharacter>(LocalController->GetCharacter());
		if (IsValid(Character))
		{
			// UnEquip Item Through Controller
			Character->DetachEquipment_Server(Item_ID);

			EquipItem_Image->SetVisibility(ESlateVisibility::Hidden);
			Item_ID = FName("Empty");
			Item_Name = FName("None");
			Item_Texture2D = nullptr;
		}
	}

	//if (IsValid(LocalController))
	//{
	//	APlayer_Controller* controller = Cast<APlayer_Controller>(LocalController);
	//	if (IsValid(controller))
	//	{
	//		// UnEquip Item Through Controller
	//		controller->DetachEquipment_Server(Item_ID);

	//		EquipItem_Image->SetVisibility(ESlateVisibility::Hidden);
	//		Item_ID = FName("Empty");
	//		Item_Name = FName("None");
	//		Item_Texture2D = nullptr;
	//	}
	//}
}
