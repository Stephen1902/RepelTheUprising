// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "RTUContainerActor.generated.h"

UCLASS()
class REPELTHEUPRISING_API ARTUContainerActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTUContainerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<UStaticMeshComponent> BaseMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<UStaticMeshComponent> LidMeshComp;

	// Widget to be displayed on screen on opening this container
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TSubclassOf<class URTUContainerWidget> ContainerWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class URTUInventoryComponent> InventoryComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual FText LookAt_Implementation() override;
	virtual void InteractWith_Implementation(ARepelTheUprisingCharacter* CharacterWhoInteracted) override;
};