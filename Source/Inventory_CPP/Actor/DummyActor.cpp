// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DummyActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ADummyActor::ADummyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh"));
	const FTransform MeshTransform = FTransform(
		FRotator(0.f, -90.f, 0.f), FVector(0.f, 0.f, -100.f), FVector(1.f)
	);
	Mesh->SetRelativeTransform(MeshTransform);
	Mesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(Mesh);

	SceneCapture2D = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("SceneCapture"));
	SceneCapture2D->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ADummyActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		UE_LOG(LogTemp, Display, TEXT("ADummyActor: Mesh Set up."));
		ACharacter* player = UGameplayStatics::GetPlayerCharacter(World, 0);
		if (IsValid(player))
		{		
			if (IsValid(player->GetMesh()->SkeletalMesh))
			{
				Mesh->SetSkeletalMesh(player->GetMesh()->SkeletalMesh);
				if(player->GetMesh()->GetAnimInstance()->GetClass())
					Mesh->SetAnimInstanceClass(player->GetMesh()->GetAnimInstance()->GetClass());
			}
		}
		else
			UE_LOG(LogTemp, Display, TEXT("ADummyActor: Filed to get player Character."));
	}
}

// Called every frame
void ADummyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

