// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "Cube/Factory/ItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/DragDropOperation.h"
#include "IntemDragDropOperation.h"
#include "Cube/Factory/InventoryManager.h"
#include "InventoryWidget.h"

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

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
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
    
    UIntemDragDropOperation* DragOp = Cast<UIntemDragDropOperation>(InOperation);
    if (!DragOp) return false;

    const int32 From = DragOp->SourceIndex;
    const int32 To = SlotIndex;
    UE_LOG(LogTemp, Error, TEXT("Drop %d    %d"), From, To);

    
    FInventoryItem ItemA = InventoryManager->ItemsArray[SlotIndex], ItemB = DragOp->SourseInventory->ItemsArray[DragOp->SourceIndex];
    
    InventoryManager->RemoveItem(SlotIndex);
    DragOp->SourseInventory->RemoveItem(DragOp->SourceIndex);
    InventoryManager->AddItem(ItemB, SlotIndex);
    DragOp->SourseInventory->AddItem(ItemA, DragOp->SourceIndex);

 //   if (UInventoryWidget && (From != To))
  //  {
     //   UInventoryWidget->ItemsArray.Swap(From, To);
        // Обновляем UI
        //GetOwningPlayer()->GetHUD()->Something... ?
            // Обычно вызываешь RefreshInventory() в основном виджете
 //   }

    return true;
}
