// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UInventoryManager;
class UItemTooltip;

UCLASS()
class CUBE_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

private:
    FVector2D GetWidgetScreenPosition() const;
protected:
    UPROPERTY()
    class UItemInfo* MyInfo;

    UPROPERTY();
    TSubclassOf <UInventorySlotWidget> MyClass;

    UPROPERTY();
    bool IsTooltipOn = 0;

    UPROPERTY();
    float TooltipDelayTimer = 0;

    UPROPERTY();
    bool IsOverlap = 0;

    UPROPERTY();
    UItemTooltip* ActiveTooltip = nullptr;

public:   
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativeDestruct() override;

    UPROPERTY()
    int32 SlotIndex = 0;

    UPROPERTY()
    UInventoryManager* InventoryManager;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;
       
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemCountText;

    UPROPERTY(meta = (BindWidget))
    class UOverlay* ItemOverlay;

    UPROPERTY(EditAnywhere);
    TSubclassOf <UItemTooltip> TooltipClass;

    void SetItem(class UItemInfo* ItemInfo, int Count);
    void SetItem();


    UFUNCTION(BlueprintCallable)
    void SetConfig(TSubclassOf <UInventorySlotWidget> MyBluprintClass, UInventoryManager* MyInventoryManager, int32 MySlotIndex);

    UPROPERTY(EditAnywhere)
    float TooltipDelay = 1.0f;

    UFUNCTION(BlueprintCallable)
    void ShowTooltip();
    UFUNCTION(BlueprintCallable)
    void HideTooltip();

};
