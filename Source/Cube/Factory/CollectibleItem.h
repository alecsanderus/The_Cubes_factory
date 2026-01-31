// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectibleItem.generated.h"

class UInteractComponent;
class UItemInfo;

UCLASS()
class CUBE_API ACollectibleItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollectibleItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere)
	UInteractComponent* InteractComponent;

	UPROPERTY(EditAnywhere)
	UItemInfo* ThisItem;

	UPROPERTY(EditAnywhere)
	int amount = 1;

	UFUNCTION (BlueprintCallable)
	void SetItem(UItemInfo* Item, int amoun = 1);

	UFUNCTION(BlueprintCallable)
	void Interact(UObject* Caller);
};
