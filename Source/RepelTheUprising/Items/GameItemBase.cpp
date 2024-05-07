// Copyright 2024 DME Games

#include "GameItemBase.h"

#define InteractTrace ECC_GameTraceChannel2

// Sets default values
AGameItemBase::AGameItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComponent->SetupAttachment(SceneComponent);
	MeshComponent->SetCollisionResponseToChannel(InteractTrace, ECR_Block);
	
	TextToReturn = FText::FromString("Default Text");
	
}

// Called when the game starts or when spawned
void AGameItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText AGameItemBase::LookAt_Implementation()
{
	return TextToReturn;	
}

