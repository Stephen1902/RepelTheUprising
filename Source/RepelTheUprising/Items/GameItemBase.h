// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RepelTheUprising/Framework/InteractInterface.h"
#include "GameItemBase.generated.h"

UCLASS()
class REPELTHEUPRISING_API AGameItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	FText TextToReturn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	class URTUItemComponent* ItemComponent;
	
public:	
	virtual FText LookAt_Implementation() override;

	void SetItemInfo(const FName INItemID, const int32 InQuantity);
};
