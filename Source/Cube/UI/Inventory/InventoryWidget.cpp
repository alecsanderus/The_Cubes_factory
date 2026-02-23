#include "InventoryWidget.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "Cube/DebugMacros.h"
#include "InventorySlotWidget.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    DEBUG_CHECK_RETURN("IventoryWidget", "objects classes",SlotWidgetClass && ItemGrid)


}

void UInventoryWidget::SetInventoryManager(UInventoryManager* NewInventoryManager)
{
    DEBUG_CHECK_RETURN("IventoryWidget", "IventoryManager", NewInventoryManager)
    InventoryManager = NewInventoryManager;
    InventoryManager->OnItemsChanged.AddDynamic(this, &UInventoryWidget::RefreshInventory);  
}



void UInventoryWidget::RefreshInventory()
{
 
    DEBUG_CHECK_RETURN ("IventoryWidget", "objects classes", InventoryManager && SlotWidgetClass && ItemGrid)

   if (!InventoryManager || !SlotWidgetClass || !ItemGrid)
        return;
    

    ItemGrid->ClearChildren();



    int32 size = InventoryManager->ItemsArray.Num();
    for (int32 i = 0; i < size; i++)
    {
        const FInventoryItem& Item = InventoryManager->ItemsArray[i];

      
        UInventorySlotWidget* Slote = CreateWidget<UInventorySlotWidget>(GetWorld(), SlotWidgetClass);
        
        Slote->SetItem(Item.Object, Item.Count);
        Slote->SetConfig(SlotWidgetClass, InventoryManager, i);
        

       
        int Row = i / GridSize;
        int Col = i % GridSize;

        //ItemGrid->AddChildToUniformGrid(Slote, Row, Col);
        UGridSlot* GridSlot = Cast<UGridSlot>(ItemGrid->AddChildToGrid(Slote, Row, Col));
        if (GridSlot)
        {
           
           // GridSlot->SetPadding(FMargin(2, 5, 2, 5));
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
        }
    }

   
}


