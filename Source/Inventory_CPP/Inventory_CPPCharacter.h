// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structure/ItemStructure.h"
#include "PlayerEnums.h"
#include "Inventory_CPPCharacter.generated.h"

UCLASS(config=Game)
class AInventory_CPPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AInventory_CPPCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	class APlayer_Controller* Controller;

/*
//////////////////
	Inventory
//////////////////
*/

public:
	class UInventoryComponent* InventoryComponent;

	UPROPERTY()
	UDataTable* ItemDB;

	void Inventory();

	void Interact();

	void UpdateCurrentQuickSlot(FKey key);

	UFUNCTION(Server, reliable)
	void Interact_Server(UInventoryComponent* InvComp);
	void Interact_Server_Implementation(UInventoryComponent* InvComp);


/*
//////////////////
	Equipment
//////////////////
*/
	UFUNCTION(Server, reliable)
	void AttachEquipment_Server(FName ItemID);
	void AttachEquipment_Server_Implementation(FName ItemID);

	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	void AttachEquipment_Multicast(FName ItemID);
	void AttachEquipment_Multicast_Implementation(FName ItemID);

	UFUNCTION(Server, reliable)
	void DetachEquipment_Server(FName ItemID);
	void DetachEquipment_Server_Implementation(FName ItemID);

	EPlayerWeaponType WeaponType = EPlayerWeaponType::EPWT_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Helmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Armor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Glove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Pants;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Shoes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Weapon;
	

/*
///////////////////////////////////
	Input & Animation & Montages 
///////////////////////////////////
*/
	UFUNCTION(Server, reliable, BlueprintCallable)
	void ChangeAnimInstance_Server(UClass* AnimInstance);
	void ChangeAnimInstance_Server_Implementation(UClass* AnimInstance);

	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	void ChangeAnimInstance_Multicast(UClass* AnimInstance);
	void ChangeAnimInstance_Multicast_Implementation(UClass* AnimInstance);

	UFUNCTION(Server, reliable, BlueprintCallable)
	void PlayMontage_Server(UAnimMontage* MontageToPlay);
	void PlayMontage_Server_Implementation(UAnimMontage* MontageToPlay);

	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	void PlayMontage_Multicast(UAnimMontage* MontageToPlay);
	void PlayMontage_Multicast_Implementation(UAnimMontage* MontageToPlay);

	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool bMontagePlaying = false;

	/*
	////////////////////
		Left Shift
	///////////////////
	*/
	UFUNCTION()
	void LeftShiftPress();

	UFUNCTION()
	void LeftShiftRelease();

	bool LeftShiftPressed = false;


	/*
	////////////////////
		Left Click
	///////////////////
	*/
	UFUNCTION()
	void LeftClick();

	UPROPERTY(EditAnywhere, Category = RightClick, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PunchMontage;

	UPROPERTY(EditAnywhere, Category = RightClick, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AxeAttackMontage;

	UPROPERTY(EditAnywhere, Category = RightClick, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AxeComboAttackMontage;

	/*
	////////////////////
		Right Click
	///////////////////
	*/
	UFUNCTION()
	void RightClickPress();

	UFUNCTION()
	void RightClickRelease();

	bool bRightClick = false;

	UPROPERTY(EditAnywhere, Category = RightClick, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AxeGuardMontage;

	UPROPERTY(EditAnywhere, Category = RightClick, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AxeGuardHitMontage;

};

