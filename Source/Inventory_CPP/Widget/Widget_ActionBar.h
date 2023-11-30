// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ActionBar.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UWidget_ActionBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeActionBar(class UInventoryComponent* InvComp, int SlotIndex);
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void Use();

	UFUNCTION()
	void Throw();

	UFUNCTION()
	void Close();

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	class UVerticalBox* Box_ActionBar;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Use;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Throw;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;

private:
	class UInventoryComponent* Inventory;
	int SlotIdx = -1;
	bool Initialized = false;
};
