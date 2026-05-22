// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryCharWidget.generated.h"


UCLASS()
class CUBE_API UInventoryCharWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
		
	class UInventoryManager* InventoryManager;
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RefreshInventory();

	UFUNCTION(BlueprintCallable)
	void SetInventoryManager(UInventoryManager* NewInventoryManager);

	
	UPROPERTY(meta = (BindWidget))
	UInventorySlotWidget* Weapon_Slot;

	UPROPERTY(meta = (BindWidget))
	UInventorySlotWidget* Armor_Slot;

	UPROPERTY(meta = (BindWidget))
	UInventorySlotWidget* Helmet_Slot;

	UPROPERTY(EditAnywhere)
	TSubclassOf <UInventorySlotWidget> SlotWidgetClass;
};
