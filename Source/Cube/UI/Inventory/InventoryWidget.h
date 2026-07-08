#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

class UInventoryManager;
class UInventorySlotWidget;

UCLASS()
class CUBE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 GridSize = 13;


	UPROPERTY()
	UInventoryManager* InventoryManager;

	void CreateAndAddSlot(int32 Index);

	UPROPERTY()
	TArray <UInventorySlotWidget*> InventorySlots;
public:
	virtual void NativeConstruct() override;

	void RefreshInventory(int32 Index);
	void RefreshInventory();


	UFUNCTION(BlueprintCallable)
	void SetInventoryManager(UInventoryManager* NewInventoryManager);

	UPROPERTY (EditAnywhere)
	TSubclassOf <UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UGridPanel* ItemGrid;
};
