// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player_Character.h"
#include "Component/InventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayer_Character::APlayer_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_SpringArm->SetupAttachment(RootComponent);

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_SpringArm);

	m_InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("Inventory"));
}

// Called when the game starts or when spawned
void APlayer_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayer_Character::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayer_Character::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayer_Character::Jump);

}

void APlayer_Character::MoveForward(float value)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector forwardVector = GetActorForwardVector() * value;
	AddMovementInput(forwardVector);
}

void APlayer_Character::MoveRight(float value)
{
	FVector rightVector = GetActorRightVector() * value;
	AddMovementInput(rightVector);
}

void APlayer_Character::Move(float value)
{
}

void APlayer_Character::Jump()
{
	Super::Jump();
}

