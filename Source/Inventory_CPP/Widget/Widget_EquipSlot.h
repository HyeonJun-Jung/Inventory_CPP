// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_EquipSlot.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_EquipSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void InitializeSlot(class UInventoryComponent* InventoryComp);

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// Redirect On Preview Mouse Button Down in Blueprint 
	// If On Preview Mouse Button Down is Called in Blueprint, NativeOnMouseButtonDown is not called.
	UFUNCTION(BlueprintCallable)
	FEventReply RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);


	// Detach Equipment From Charactrer
	void DetachEquipment();

public:
	class UInventoryComponent* InvComp;

	UPROPERTY(meta = (BindWidget))
	class UImage* EquipItem_Image;

	UPROPERTY(meta = (BindWidget))
	class UButton* EquipSlot_Button;

	FName Item_ID = FName("Empty");
	FName Item_Name = FName("None");
	UTexture2D* Item_Texture2D;
};
