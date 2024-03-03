// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Inventory_CPPCharacter.h"
#include "AnimInst_Base.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_CPP_API UAnimInst_Base : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAnimInst_Base();

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

private:
	AInventory_CPPCharacter* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	bool bRightClick;
};
