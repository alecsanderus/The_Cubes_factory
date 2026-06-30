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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FGuid MyMachineId;
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
	void SetInventoryManager(FGuid MachineId);
	virtual void NativeConstruct() override;

	UFUNCTION (BlueprintCallable)
	void ItemsChanged();
};
