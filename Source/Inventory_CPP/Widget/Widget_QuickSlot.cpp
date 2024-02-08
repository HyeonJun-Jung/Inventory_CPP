// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_QuickSlot.h"
#include "Widget/Widget_Inventory_Slot.h"
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

bool UWidget_QuickSlot::Initialize()
{
	Super::Initialize();
	
	UDataTable* BP_ItemDB = LoadObject<UDataTable>(this, TEXT("/Game/Inventory/Data/DB_ItemData.DB_ItemData"));
	if (IsValid(BP_ItemDB))
	{
		ItemDB = BP_ItemDB;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : Can't Get DataTable."))
	}

	FStringClassReference ChestWidgetClassRef(TEXT("/Game/Inventory/Widget/WBP_ActionBar.WBP_ActionBar_C"));
	ActionBar_WidgetClass = ChestWidgetClassRef.TryLoadClass<UWidget_ActionBar>();

	return true;
}

void UWidget_QuickSlot::InitializeSlot(int SlotNum, UInventoryComponent* InventoryComp)
{
	SlotIdx = SlotNum;
	InvComp = InventoryComp;
}

void UWidget_QuickSlot::UpdateSlot(FItemStructure item, uint8 quantity)
{
	Item_ID = item.ID;
	Item_Name = item.Name;
	Item_Category = item.Category;
	Item_Texture2D = item.Icon;
	Quantity = quantity;

	QuickSlot_Image->SetBrushFromTexture(item.Icon);
	QuickSlot_Quantity->SetText(UKismetTextLibrary::Conv_IntToText(quantity));

	if (Quantity != 0)
	{
		QuickSlot_Image->SetVisibility(ESlateVisibility::Visible);
		QuickSlot_Quantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		QuickSlot_Image->SetVisibility(ESlateVisibility::Hidden);
		QuickSlot_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidget_QuickSlot::UpdateSlot(FName Name, FName Category, UTexture2D* Icon, uint8 quantity)
{
	Item_Name = Name;
	Item_Category = Category;
	Item_Texture2D = Icon;
	Quantity = quantity;

	QuickSlot_Image->SetBrushFromTexture(Icon);
	QuickSlot_Quantity->SetText(UKismetTextLibrary::Conv_IntToText(quantity));

	if (Quantity != 0)
	{
		QuickSlot_Image->SetVisibility(ESlateVisibility::Visible);
		QuickSlot_Quantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		QuickSlot_Image->SetVisibility(ESlateVisibility::Hidden);
		QuickSlot_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

FReply UWidget_QuickSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : NativeOnMouseButtonDown HAS BEEN CALLED."));

	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : SlotIdx Is %d."), SlotIdx);
		if (!Item_Name.IsEqual(FName("None")))
		{
			UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : ItemID Is %s."), *Item_Name.ToString());
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton);
		}
	}

	return Reply.NativeReply;
}

FReply UWidget_QuickSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : NativeOnMouseButtonUp HAS BEEN CALLED."));

	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	return Reply.NativeReply;
}

void UWidget_QuickSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : NativeOnDragDetected HAS BEEN CALLED."));

	if (IsValid(OutOperation))
		UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : Drag Again."));

	USlotDragDrop* DragDropOperation = NewObject<USlotDragDrop>();
	OutOperation = DragDropOperation;

	DragDropOperation->DefaultDragVisual = this;
}

bool UWidget_QuickSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UE_LOG(LogTemp, Warning, TEXT("UWidget_QuickSlot : NativeOnDrop SlotIdx : %d."), SlotIdx);

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
				controller->Transfer_Slot_InvToQuick_Server(SourceIdx, SlotIdx);
		}

		return true;
	}

	UWidget_QuickSlot* QuickSlot = Cast<UWidget_QuickSlot>(InOperation->DefaultDragVisual);
	if (IsValid(QuickSlot))
	{
		int SourceIdx = QuickSlot->SlotIdx;

		APlayerController* LocalController = GetGameInstance()->GetFirstLocalPlayerController();
		if (IsValid(LocalController))
		{
			APlayer_Controller* controller = Cast<APlayer_Controller>(LocalController);
			if (IsValid(controller))
				controller->Transfer_Slot_QuickToQuick_Server(SourceIdx, SlotIdx);
		}

		return true;
	}

	return true;
}

FEventReply UWidget_QuickSlot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FEventReply();
}
