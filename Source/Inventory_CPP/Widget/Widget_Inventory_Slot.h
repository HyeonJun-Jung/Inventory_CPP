// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Inventory_Slot.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_Inventory_Slot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void InitializeSlot(int SlotNum, class UInventoryComponent* InventoryComp);
	void UpdateSlot(FName Name, UTexture2D* Icon, uint8 quantity);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;



public:
	class UInventoryComponent* InvComp;

	UPROPERTY()
	class UDataTable* ItemDB;

	UPROPERTY(meta = (BindWidget))
	class UImage* Item_Image;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ItemSlot_Button;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Item_Quantity;

	int SlotIdx = 0; 
	FName Item_Name = FName("Empty");
	UTexture2D* Item_Texture2D;
	uint8 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf< UWidget_Inventory_Slot> DragVisualClass;
};
