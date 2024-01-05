// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory_CPPCharacter.h"
#include "Player_Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Component/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// AInventory_CPPCharacter

AInventory_CPPCharacter::AInventory_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Mesh for Equipment
	Helmet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Helmet"));
	Helmet->SetupAttachment(GetMesh(), FName("Socket_Head"));

	Armor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Armor"));
	Armor->SetupAttachment(GetMesh());

	Glove = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Glove"));
	Glove->SetupAttachment(GetMesh());

	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Pants->SetupAttachment(GetMesh());

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	Shoes->SetupAttachment(GetMesh());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInventory_CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Relate to Inventory Key binding
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AInventory_CPPCharacter::Interact);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AInventory_CPPCharacter::Inventory);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AInventory_CPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AInventory_CPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AInventory_CPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AInventory_CPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AInventory_CPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AInventory_CPPCharacter::TouchStopped);
}

void AInventory_CPPCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter: %s"), *GetName());
	Controller = Cast<APlayer_Controller>(GetController());
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter: Can't Cast To APlayer_Controller"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter: APlayer_Controller : %s"), *Controller->GetName());
	}

	InventoryComponent = Controller->GetInventoryComponent();
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter: Can't Get InventoryComponent"));
	}
	else
		UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter: Success to Get InventoryComponent"));

	// Set Equipment Master Pose Component
	Armor->SetMasterPoseComponent(GetMesh());
	Pants->SetMasterPoseComponent(GetMesh());
	Glove->SetMasterPoseComponent(GetMesh());
	Shoes->SetMasterPoseComponent(GetMesh());
}
	

void AInventory_CPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInventory_CPPCharacter::Inventory()
{
	if (!Controller) return;
	Controller->ShowInventory();
}

void AInventory_CPPCharacter::Interact()
{
	if (IsValid(InventoryComponent))
		InventoryComponent->Interact_Server();
	else
	{
		if (IsValid(Controller))
		{
			InventoryComponent = Controller->GetInventoryComponent();
			if (IsValid(InventoryComponent))
				InventoryComponent->Interact_Server();
			else
				UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter : InventoryComponent Is Not Valid."));
		}
		else
		{
			Controller = Cast<APlayer_Controller>(GetController());
			if (IsValid(Controller))
			{
				InventoryComponent = Controller->GetInventoryComponent();
				if (IsValid(InventoryComponent))
					InventoryComponent->Interact_Server();
				else
					UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter : InventoryComponent Is Not Valid."));
			}
			else
				UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter : Controller Is Not Valid."));
		}
	}

}

void AInventory_CPPCharacter::Interact_Server_Implementation(UInventoryComponent* InvComp)
{
	UE_LOG(LogTemp, Display, TEXT("Interact_Server_Implementation HAS BEEN CALLED"));
	
	if (IsValid(InvComp))
		InvComp->Interact();

	/*if (IsValid(InventoryComponent))
		InventoryComponent->Interact();
	else
	{
		if (IsValid(Controller))
		{
			InventoryComponent = Controller->GetInventoryComponent();
			if(IsValid(InventoryComponent))
				InventoryComponent->Interact();
			else
				UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter : InventoryComponent Is Not Valid."));
		}
		else
		{
			Controller = Cast<APlayer_Controller>(GetController());
			if (IsValid(Controller))
			{
				InventoryComponent = Controller->GetInventoryComponent();
				if (IsValid(InventoryComponent))
					InventoryComponent->Interact();
				else
					UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter : InventoryComponent Is Not Valid."));
			}
			else
				UE_LOG(LogTemp, Display, TEXT("AInventory_CPPCharacter : Controller Is Not Valid."));
		}
	}*/
}

void AInventory_CPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AInventory_CPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AInventory_CPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AInventory_CPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AInventory_CPPCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AInventory_CPPCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
