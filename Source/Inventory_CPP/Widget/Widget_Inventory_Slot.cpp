// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Inventory_Slot.h"
#include "Widget/Widget_EquipSlot.h"
#include "Widget/Widget_ActionBar.h"
#include "Widget/SlotDragDrop.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Component/InventoryComponent.h"
#include "Player_Controller.h"
#include <Kismet/KismetTextLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Engine/DataTable.h>

bool UWidget_Inventory_Slot::Initialize()
{
	Super::Initialize();

	UDataTable* BP_ItemDB = LoadObject<UDataTable>(this, TEXT("/Game/Inventory/Data/DB_ItemData.DB_ItemData"));
	if (IsValid(BP_ItemDB))
	{
		ItemDB = BP_ItemDB;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't Get DataTable."))
	}

	FStringClassReference ChestWidgetClassRef(TEXT("/Game/Inventory/Widget/WBP_ActionBar.WBP_ActionBar_C"));
	ActionBar_WidgetClass = ChestWidgetClassRef.TryLoadClass<UWidget_ActionBar>();

	return true;
}

void UWidget_Inventory_Slot::InitializeSlot(int SlotNum, class UInventoryComponent* InventoryComp)
{
	InvComp = InventoryComp;
	this->SlotIdx = SlotNum;
}

void UWidget_Inventory_Slot::UpdateSlot(FItemStructure item, uint8 quantity)
{
	Item_ID = item.ID;
	Item_Name = item.Name;
	Item_Category = item.Category;
	Item_Texture2D = item.Icon;
	Quantity = quantity;

	Item_Image->SetBrushFromTexture(item.Icon);
	Item_Quantity->SetText(UKismetTextLibrary::Conv_IntToText(quantity));

	if (Quantity != 0)
	{
		Item_Image->SetVisibility(ESlateVisibility::Visible);
		Item_Quantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Item_Image->SetVisibility(ESlateVisibility::Hidden);
		Item_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidget_Inventory_Slot::UpdateSlot(FName Name, FName Category, UTexture2D* Icon, uint8 quantity)
{
	Item_Name = Name;
	Item_Category = Category;
	Item_Texture2D = Icon;
	Quantity = quantity;

	Item_Image->SetBrushFromTexture(Icon);
	Item_Quantity->SetText(UKismetTextLibrary::Conv_IntToText(quantity));

	if (Quantity != 0)
	{
		Item_Image->SetVisibility(ESlateVisibility::Visible);
		Item_Quantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Item_Image->SetVisibility(ESlateVisibility::Hidden);
		Item_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

FReply UWidget_Inventory_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : NativeOnMouseButtonDown HAS BEEN CALLED."));

	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : SlotIdx Is %d."), SlotIdx);
		if (!Item_Name.IsEqual(FName("None")))
		{			
			UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : ItemID Is %s."), *Item_Name.ToString());
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton);
		}
	}
	
	return Reply.NativeReply;
}

FReply UWidget_Inventory_Slot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : NativeOnMouseButtonUp HAS BEEN CALLED."));

	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return Reply.NativeReply;
}

void UWidget_Inventory_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : NativeOnDragDetected HAS BEEN CALLED."));

	if(IsValid(OutOperation))
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : Drag Again."));

	USlotDragDrop* DragDropOperation = NewObject<USlotDragDrop>();
	OutOperation = DragDropOperation;

	if (!DragVisualClass)
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : Should Set DragVisualClass."));

	DragDropOperation->DefaultDragVisual = this;
}

bool UWidget_Inventory_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : NativeOnDrop SlotIdx : %d."), SlotIdx);

	UWidget_Inventory_Slot* DragVisualWidget = Cast<UWidget_Inventory_Slot>(InOperation->DefaultDragVisual);
	if (IsValid(DragVisualWidget))
	{
		UInventoryComponent* SourceInv = DragVisualWidget->InvComp;
		int SourceIdx = DragVisualWidget->SlotIdx;

		//InvComp->Transfer_Slot_Server(SourceInv, SourceIdx, SlotIdx);

		APlayerController* LocalController = GetGameInstance()->GetFirstLocalPlayerController();
		if (IsValid(LocalController))
		{
			APlayer_Controller* controller = Cast<APlayer_Controller>(LocalController);
			if (IsValid(controller))
				controller->Transfer_Slot_Server(SourceInv, SourceIdx, InvComp, SlotIdx);
		}

		return true;
	}

	UWidget_EquipSlot* EquipSlot = Cast<UWidget_EquipSlot>(InOperation->DefaultDragVisual);
	if (IsValid(EquipSlot))
	{
		EquipSlot->DetachEquipment();	

		return true;
	}

	return true;
}

FEventReply UWidget_Inventory_Slot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : NativeOnMouseButtonDown HAS BEEN CALLED."));

	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : SlotIdx Is %d."), SlotIdx);
		if (!Item_Name.IsEqual(FName("None")))
		{
			UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : ItemID Is %s."), *Item_Name.ToString());
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		if (!Item_Name.IsEqual(FName("None")))
		{
			if (IsValid(ActionBar_WidgetClass))
			{
				 UWidget_ActionBar* actionBar = CreateWidget<UWidget_ActionBar>(this, ActionBar_WidgetClass);
				 actionBar->InitializeActionBar(InvComp, SlotIdx);
				 actionBar->AddToViewport();

				 FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
				 actionBar->SetPositionInViewport(MousePosition);
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : Failed to Craete ActionBar."), SlotIdx);
		}
	}

	return Reply;
}
