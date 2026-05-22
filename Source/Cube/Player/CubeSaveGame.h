// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "CubeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CUBE_API UCubeSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FInventoryItem> ItemsArray;
};
