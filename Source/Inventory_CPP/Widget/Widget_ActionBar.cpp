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

	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	// UCanvasPanelSlot* BoxAsSlot = Cast<UCanvasPanelSlot>(Box_ActionBar->Slot);
	 UCanvasPanelSlot* BoxAsSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Box_ActionBar); 
	if (IsValid(BoxAsSlot))
		BoxAsSlot->SetPosition(MousePosition);
	else
		UE_LOG(LogTemp, Warning, TEXT("UWidget_ActionBar : Failed to SlotAsCanvasSlot."));
}

void UWidget_ActionBar::Use()
{
	if (!Initialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_ActionBar : Not Initialized."));
		return;
	}
}

void UWidget_ActionBar::Throw()
{
	if (!Initialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_ActionBar : Not Initialized."));
		return;
	}
}

void UWidget_ActionBar::Close()
{
	if (!Initialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_ActionBar : Not Initialized."));
		return;
	}
}
