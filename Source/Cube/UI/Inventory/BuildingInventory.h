#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingInventory.generated.h"

UCLASS()
class CUBE_API UBuildingInventory : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	TArray <FName> SlotInNames;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FName> SlotOutNames;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray <class UInventorySlotWidget*> SlotsIn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray <class UInventorySlotWidget*> SlotsOut;


	UPROPERTY()
	class UInventoryManager* InventoryManager;
	
	UFUNCTION()
	void SetInventoryManager(UInventoryManager* Manager);
	virtual void NativeConstruct() override;
};
