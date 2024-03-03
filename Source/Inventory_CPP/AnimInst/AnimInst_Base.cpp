// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst/AnimInst_Base.h"

UAnimInst_Base::UAnimInst_Base()
{
}

void UAnimInst_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AInventory_CPPCharacter>(GetOwningActor());
}

void UAnimInst_Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Character))
	{
		bRightClick = Character->bRightClick;
	}
}
