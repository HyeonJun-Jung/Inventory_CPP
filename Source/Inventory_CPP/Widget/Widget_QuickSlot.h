// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structure/ItemStructure.h"
#include "Widget_QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_QuickSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void InitializeSlot(int SlotNum, class UInventoryComponent* InventoryComp);
	void UpdateSlot(FItemStructure item, uint8 quantity);
	void UpdateSlot(FName Name, FName Category, UTexture2D* Icon, uint8 quantity);
	void UpdateSlot(class UInventoryComponent* InventoryComp) { InvComp = InventoryComp; }

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// Redirect On Preview Mouse Button Down in Blueprint 
	// If On Preview Mouse Button Down is Called in Blueprint, NativeOnMouseButtonDown is not called.
	UFUNCTION(BlueprintCallable)
	FEventReply RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

public:
	class UInventoryComponent* InvComp;

	UPROPERTY()
	class UDataTable* ItemDB;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* QuickSlot_Image;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* QuickSlot_Button;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* QuickSlot_Quantity;

	int SlotIdx = 0;
	FName Item_ID = FName("Empty");
	FName Item_Name = FName("None");
	FName Item_Category = FName("None");
	UTexture2D* Item_Texture2D;
	uint8 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UWidget_QuickSlot> DragVisualClass;

	UClass* ActionBar_WidgetClass;
};
