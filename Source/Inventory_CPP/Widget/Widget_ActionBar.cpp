// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_ActionBar.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Component/InventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UWidget_ActionBar::InitializeActionBar(UInventoryComponent* InvComp, int SlotIndex)
{
	Inventory = InvComp;
	SlotIdx = SlotIndex;

	Initialized = true;
}

void UWidget_ActionBar::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Use->OnClicked.AddUniqueDynamic(this, &UWidget_ActionBar::Use);
	Button_Throw->OnClicked.AddUniqueDynamic(this, &UWidget_ActionBar::Throw);
	Button_Close->OnClicked.AddUniqueDynamic(this, &UWidget_ActionBar::Close);
}

void UWidget_ActionBar::Use()
{
	if (!Initialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_ActionBar : Not Initialized."));
		return;
	}

	Inventory->RemoveItem_Server(SlotIdx, true, 1);
}

void UWidget_ActionBar::Throw()
{
	if (!Initialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_ActionBar : Not Initialized."));
		return;
	}

	Inventory->RemoveItem_Server(SlotIdx, false, 1);
}

void UWidget_ActionBar::Close()
{
	if (!Initialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_ActionBar : Not Initialized."));
		return;
	}
}
