// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CubeGameInstance.generated.h"

class UInventoryManager;

UCLASS()
class CUBE_API UCubeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UPROPERTY()
	UInventoryManager* PlayerInventory;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();
};
