// Copyright 2024 DME Games

#include "RTUContainerActor.h"
#include "RepelTheUprising/Player/RepelTheUprisingCharacter.h"
#include "RepelTheUprising/Components/RTUInventoryComponent.h"
#include "RepelTheUprising/Components/RTUItemComponent.h"
#include "RepelTheUprising/Widgets/RTUContainerWidget.h"

#define InteractTrace ECC_GameTraceChannel2

// Sets default values
ARTUContainerActor::ARTUContainerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(SceneComp);

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMeshComp->SetupAttachment(SceneComp);
	BaseMeshComp->SetCollisionResponseToChannel(InteractTrace, ECR_Block);

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lid Mesh"));
	LidMeshComp->SetupAttachment(BaseMeshComp);
	LidMeshComp->SetCollisionResponseToChannel(InteractTrace, ECR_Block);
	
	InventoryComp = CreateDefaultSubobject<URTUInventoryComponent>(TEXT("Inventory Component"));

	static ConstructorHelpers::FClassFinder<URTUContainerWidget> FoundWidget(TEXT("'/Game/UI/W_ContainerWidget'"));
	if (FoundWidget.Succeeded())
	{
		ContainerWidget = FoundWidget.Class;
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void ARTUContainerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTUContainerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText ARTUContainerActor::LookAt_Implementation()
{
	const FString StringToReturn = "Press E to Open Container";
	return FText::FromString(StringToReturn);
}

void ARTUContainerActor::InteractWith_Implementation(ARepelTheUprisingCharacter* CharacterWhoInteracted)
{
	//IInteractInterface::InteractWith_Implementation(CharacterWhoInteracted);

/*	if (GetOwner() == nullptr)
	{
		//Owner = CharacterWhoInteracted;*/
		if (ContainerWidget)
		{
			CharacterWhoInteracted->AddContainerToHUD(ContainerWidget, InventoryComp);
//			URTUContainerWidget* ContainerWidgetRef = Cast<URTUContainerWidget>(ContainerWidget.GetDefaultObject());
//			ContainerWidgetRef->SetInventoryComp(InventoryComp, CharacterWhoInteracted->GetInventoryComp());
		}/*
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Someone else is interacting with this container"));
	}
	*/
	UE_LOG(LogTemp, Warning, TEXT("Container is interacted with"));
}
