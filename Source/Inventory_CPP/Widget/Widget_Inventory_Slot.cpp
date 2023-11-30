// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_Inventory_Slot.h"
#include "Widget/SlotDragDrop.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Component/InventoryComponent.h"
#include "Player_Controller.h"
#include <Kismet/KismetTextLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
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

	return true;
}

void UWidget_Inventory_Slot::InitializeSlot(int SlotNum, class UInventoryComponent* InventoryComp)
{
	InvComp = InventoryComp;
	this->SlotIdx = SlotNum;
}

void UWidget_Inventory_Slot::UpdateSlot(FName Name, UTexture2D* Icon, uint8 quantity)
{
	Item_Name = Name;
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

	UWidget_Inventory_Slot* DragVisualWidget = CreateWidget<UWidget_Inventory_Slot>(this, DragVisualClass);
	DragVisualWidget->InvComp = this->InvComp;
	DragVisualWidget->SlotIdx = this->SlotIdx;
	DragVisualWidget->Item_Image->SetBrushFromTexture(Item_Texture2D);
	DragVisualWidget->Item_Quantity->SetText(UKismetTextLibrary::Conv_IntToText(Quantity));
	DragVisualWidget->Item_Image->SetVisibility(ESlateVisibility::Visible);
	DragVisualWidget->Item_Quantity->SetVisibility(ESlateVisibility::Visible);
	// DragDropOperation->DefaultDragVisual = DragVisualWidget;
	DragDropOperation->DefaultDragVisual = this;
}

bool UWidget_Inventory_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UE_LOG(LogTemp, Warning, TEXT("UWidget_Inventory_Slot : NativeOnDrop SlotIdx : %d."), SlotIdx);

	UWidget_Inventory_Slot* DragVisualWidget = Cast<UWidget_Inventory_Slot>(InOperation->DefaultDragVisual);
	
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