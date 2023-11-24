// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player_Character.generated.h"

class UInventoryComponent;

UCLASS()
class INVENTORY_CPP_API APlayer_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void Move(float value);

	virtual void Jump();


public:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* m_SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* m_Camera;


// Inventory
public:
	UInventoryComponent* m_InventoryComponent;

};
