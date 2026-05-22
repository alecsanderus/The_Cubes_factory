// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "Cube/Factory/Items/ItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/DragDropOperation.h"
#include "IntemDragDropOperation.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "InventoryWidget.h"
#include "ItemTooltip.h"
#include "Cube/DebugMacros.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UInventorySlotWidget::SetItem(UItemInfo* ItemInfo, int Count)
{
    if (!ItemInfo) { SetItem(); return;}
    MyInfo = ItemInfo;
    ItemCountText->SetVisibility(ESlateVisibility::Visible);
    ItemImage->SetVisibility(ESlateVisibility::Visible);
    if (!ItemInfo) return;
       
    if (Count != -1)
        ItemCountText->SetText(FText::AsNumber(Count));
    else
        ItemCountText->SetVisibility(ESlateVisibility::Hidden);
       
    if (ItemInfo->Icon)
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(ItemInfo->Icon);
        ItemImage->SetBrush(Brush);      
    }

   
}

void UInventorySlotWidget::SetItem()
{
    ItemCountText->SetVisibility(ESlateVisibility::Hidden);
    ItemImage->SetVisibility(ESlateVisibility::Hidden);
}


void UInventorySlotWidget::SetConfig(TSubclassOf < UInventorySlotWidget> MyBluprintClass, UInventoryManager* MyInventoryManager, int32 MySlotIndex)
{
    InventoryManager = MyInventoryManager;
    SlotIndex = MySlotIndex;
    MyClass = MyBluprintClass;
}

void UInventorySlotWidget::ShowTooltip()
{
    IsTooltipOn = 1;
    if (ActiveTooltip) return;
    if (!MyInfo) return;
    DEBUG_CHECK_RETURN("UInventorySlotWidget", TooltipClass);

    ActiveTooltip = CreateWidget <UItemTooltip>(this, TooltipClass);
    ActiveTooltip->AddToViewport();

    FVector2D MousePos;
    if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePos.X, MousePos.Y))
    {
        ActiveTooltip->SetPositionInViewport(MousePos + FVector2D (50,0), true);
    }
    ActiveTooltip->SetItem(MyInfo);
}

void UInventorySlotWidget::HideTooltip()
{
    IsTooltipOn = 0;
    if (ActiveTooltip)
    {
        ActiveTooltip->RemoveFromParent();
        ActiveTooltip = nullptr;
    }
}


void UInventorySlotWidget::NativeDestruct()
{
    if (ActiveTooltip && IsValid (ActiveTooltip))
    {
        ActiveTooltip->RemoveFromParent();
        ActiveTooltip = nullptr;
    }
}

void UInventorySlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (!IsOverlap) return;
    if (IsTooltipOn) return;
    TooltipDelayTimer += InDeltaTime;
    if (TooltipDelayTimer >= TooltipDelay)
    {
        IsTooltipOn = 1;
        ShowTooltip();
        TooltipDelayTimer = 0;
    }
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    UE_LOG(LogTemp, Error, TEXT("MouseDown"));
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }
    return FReply::Unhandled();           
}

void UInventorySlotWidget::NativeOnDragDetected(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent,
    UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    UE_LOG(LogTemp, Error, TEXT("DragDetected"));
    UIntemDragDropOperation* DragOp = NewObject<UIntemDragDropOperation>();
    DragOp->SourceIndex = SlotIndex;
    DragOp->SourseInventory = InventoryManager;

    UInventorySlotWidget* viso = CreateWidget<UInventorySlotWidget>(GetWorld(), MyClass);

    viso->SetItem(MyInfo,-1);
   // UInventorySlotWidget* viso = CreateWidget<UInventorySlotWidget>(GetWorld());
   //
   // viso->SetItem(MyInfo, -1);
   // viso->SetVisibility(ESlateVisibility::Hidden);
   //
    DragOp->DefaultDragVisual = viso;

    OutOperation = DragOp;
    
}

bool UInventorySlotWidget::NativeOnDrop(
    const FGeometry& InGeometry,
    const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UIntemDragDropOperation* DragOp = Cast<UIntemDragDropOperation>(InOperation);
    if (!DragOp) return false;

    const int32 From = DragOp->SourceIndex;
    const int32 To = SlotIndex;
    UE_LOG(LogTemp, Error, TEXT("Drop %d    %d"), From, To);

    
    FInventoryItem ItemA = InventoryManager->ItemsArray[SlotIndex], ItemB = DragOp->SourseInventory->ItemsArray[DragOp->SourceIndex];
    
    InventoryManager->RemoveItem(SlotIndex,1);
    DragOp->SourseInventory->RemoveItem(DragOp->SourceIndex,1);
    InventoryManager->AddItem(ItemB, SlotIndex,1,1);
    DragOp->SourseInventory->AddItem(ItemA, DragOp->SourceIndex,1,1);

    InventoryManager->CheckInventory();
    DragOp->SourseInventory->CheckInventory();

 //   if (UInventoryWidget && (From != To))
  //  {
     //   UInventoryWidget->ItemsArray.Swap(From, To);
        // Обновляем UI
        //GetOwningPlayer()->GetHUD()->Something... ?
            // Обычно вызываешь RefreshInventory() в основном виджете
 //   }

    return true;
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
    IsOverlap = 1;
    TooltipDelayTimer = 0;
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    IsOverlap = 0;
    HideTooltip();
    TooltipDelayTimer = 0;
}


FVector2D UInventorySlotWidget::GetWidgetScreenPosition() const
{
    // Получаем абсолютную позицию виджета в экранных координатах (в "пикселях Slate")
    FVector2D AbsolutePos = GetCachedGeometry().GetAbsolutePosition();

    // Получаем размер виджета
    FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();

    // Рассчитываем позицию верхнего правого угла
    return AbsolutePos + FVector2D(WidgetSize.X, 0.f);
}