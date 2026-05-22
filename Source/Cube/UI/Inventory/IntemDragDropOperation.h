// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "IntemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class CUBE_API UIntemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
  
    UPROPERTY(EditAnywhere)
    int SourceIndex;
  
    UPROPERTY(EditAnywhere)
    class UInventoryManager* SourseInventory;
};
