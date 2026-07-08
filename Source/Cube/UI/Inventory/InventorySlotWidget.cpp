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


void UInventorySlotWidget::SetConfig(UInventoryManager* MyInventoryManager, int32 MySlotIndex, bool AutoSetItem)
{
    AutoUpdateEnabled = 0;
    InventoryManager = MyInventoryManager;
    SlotIndex = MySlotIndex;
    if (AutoSetItem)
    {
        UpdateItem(-1);
    }
}

void UInventorySlotWidget::UpdateItem(int32 Index)
{
	if (Index != -1 && Index != SlotIndex) return;

    DEBUG_CHECK_RETURN(UInventorySlotWidget, InventoryManager);
	SetItem(InventoryManager->GetItem(SlotIndex).Object, InventoryManager->GetItem(SlotIndex).Count);
}

void UInventorySlotWidget::EnableAutoUpdate()
{
    AutoUpdateEnabled = 1;
	InventoryManager->OnItemsChanged.AddUObject(this, &UInventorySlotWidget::UpdateItem);
}

void UInventorySlotWidget::ShowTooltip()
{
    IsTooltipOn = 1;
    if (ActiveTooltip) return;
    if (!MyInfo) return;
    DEBUG_CHECK_RETURN(UInventorySlotWidget, TooltipClass);

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
    if (!MyInfo) return;
    UIntemDragDropOperation* DragOp = NewObject<UIntemDragDropOperation>();
    DragOp->SourceIndex = SlotIndex;
    DragOp->SourseInventory = InventoryManager;

    UInventorySlotWidget* viso = CreateWidget<UInventorySlotWidget>(GetWorld(), GetClass());

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
    if (!DragOp->SourseInventory) return false;
    if (!InventoryManager) return false;
    if (DragOp->SourceIndex < 0)  return false;

    const int32 From = DragOp->SourceIndex;
    const int32 To = SlotIndex;
    UE_LOG(LogTemp, Error, TEXT("Drop %d    %d"), From, To);


    if (From == To && InventoryManager == DragOp->SourseInventory)
        return 1;

    FInventoryItem ItemA = InventoryManager->ItemsArray[SlotIndex], ItemB = DragOp->SourseInventory->ItemsArray[DragOp->SourceIndex];

    if (ItemA.Object == ItemB.Object)
    {
        int32 TotalCount = ItemA.Count + ItemB.Count;
        FInventoryItem NewItemA = ItemA;
        NewItemA.Count = TotalCount;

        int32 CanCount = InventoryManager->CanSetItem(NewItemA, To);
        if (CanCount)
        {
            InventoryManager->SetItem(FInventoryItem(ItemA.Object, CanCount), SlotIndex, false, InventoryManager == DragOp->SourseInventory);

            DragOp->SourseInventory->SetItem(FInventoryItem((TotalCount - CanCount <= 0) ? nullptr : ItemA.Object, TotalCount - CanCount), DragOp->SourceIndex);
        }
    }
    else
    {
        bool CanSetA = (InventoryManager->CanSetItem(ItemB, To) == ItemB.Count) ? 1 : 0;
        bool CanSetB = (DragOp->SourseInventory->CanSetItem(ItemA, DragOp->SourceIndex) == ItemA.Count) ? 1 : 0;

        if (CanSetA && CanSetB)
        {
            if (!InventoryManager->SetItem(ItemB, SlotIndex, false, InventoryManager == DragOp->SourseInventory))
                InventoryManager->AddItem(ItemB, true, InventoryManager == DragOp->SourseInventory);

            if (!DragOp->SourseInventory->SetItem(ItemA, DragOp->SourceIndex))
				DragOp->SourseInventory->AddItem(ItemA, true);
        }
    }
    //InventoryManager->RemoveItem(SlotIndex, 1);
    //DragOp->SourseInventory->RemoveItem(DragOp->SourceIndex, 1);
    //InventoryManager->AddItem(ItemB, SlotIndex, 1, 1);
    //DragOp->SourseInventory->AddItem(ItemA, DragOp->SourceIndex, 1, 1);

    //InventoryManager->CheckInventory();
    //DragOp->SourseInventory->CheckInventory();

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
    FVector2D AbsolutePos = GetCachedGeometry().GetAbsolutePosition();
    FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();
    return AbsolutePos + FVector2D(WidgetSize.X, 0.f);
}