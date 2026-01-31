// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UInventoryManager;

UCLASS()
class CUBE_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    UPROPERTY()
    class UItemInfo* MyInfo;

    UPROPERTY();
    TSubclassOf <UInventorySlotWidget> MyClass;
public:
    UPROPERTY()
    int32 SlotIndex = 0;

    UPROPERTY()
    UInventoryManager* InventoryManager;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;
       
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemCountText;
       
    void SetItem(class UItemInfo* ItemInfo, int Count);
    void SetItem();
    UFUNCTION(BlueprintCallable)
    void SetConfig(TSubclassOf <UInventorySlotWidget> MyBluprintClass, UInventoryManager* MyInventoryManager, int32 MySlotIndex);
};
