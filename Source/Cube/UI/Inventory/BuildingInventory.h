#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingInventory.generated.h"

UCLASS()
class CUBE_API UBuildingInventory : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY (EditAnywhere)
	TArray <class UInventorySlotWidget*> Slots;

	UPROPERTY()
	class UInventoryManager* InventoryManager;
	
	UFUNCTION()
	void SetInventoryManager(UInventoryManager* Manager);
};
