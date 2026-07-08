#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingInventory.generated.h"

UCLASS()
class CUBE_API UBuildingInventory : public UUserWidget
{
	GENERATED_BODY()
private:
protected:
	
	bool IsUpdatingNow = 0;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid MyMachineId;


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
	void SetInventoryManager(FGuid MachineId);
	virtual void NativeConstruct() override;

	UFUNCTION (BlueprintCallable)
	void ItemsChanged(int32 Index);
};
