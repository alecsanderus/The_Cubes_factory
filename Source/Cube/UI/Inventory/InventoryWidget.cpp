#include "InventoryWidget.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "Cube/DebugMacros.h"
#include "InventorySlotWidget.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    DEBUG_CHECK_RETURN(IventoryWidget, SlotWidgetClass && ItemGrid)


}

void UInventoryWidget::SetInventoryManager(UInventoryManager* NewInventoryManager)
{
    DEBUG_CHECK_RETURN(IventoryWidget, NewInventoryManager)
    InventoryManager = NewInventoryManager;
    InventoryManager->OnItemsChanged.AddUObject(this, &UInventoryWidget::RefreshInventory);  
}



void UInventoryWidget::RefreshInventory(int32 Index)
{	
    DEBUG_CHECK_RETURN(IventoryWidget, InventoryManager && SlotWidgetClass && ItemGrid);

   if (!InventoryManager || !SlotWidgetClass || !ItemGrid)
        return;
    
 /*
    ItemGrid->ClearChildren();

   

    int32 size = InventoryManager->ItemsArray.Num();
    for (int32 i = 0; i < size; i++)
    {
        const FInventoryItem& Item = InventoryManager->ItemsArray[i];

      
        UInventorySlotWidget* Slote = CreateWidget<UInventorySlotWidget>(GetWorld(), SlotWidgetClass);
        
        Slote->SetItem(Item.Object, Item.Count);
        Slote->SetConfig(InventoryManager, i);
        

       
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
    */
    if (Index == -1)
    {
        RefreshInventory(-2);      
    }
   
    else if (Index == -2)
    {       
		int32 NewSize = InventoryManager->ItemsArray.Num();
		int32 OldSize = InventorySlots.Num();
        
        // Добавляем недостающие слоты
        for (int32 i = OldSize; i < NewSize; i++)
            CreateAndAddSlot(i);        


        for (int32 i = OldSize - 1; i >= NewSize; i--)
        {
            UInventorySlotWidget* SlotToRemove = InventorySlots[i];
                if (SlotToRemove && ItemGrid)
                    ItemGrid->RemoveChild(SlotToRemove);
        }
		InventorySlots.SetNum(NewSize);     
    }

    else
    {
        if (InventorySlots.IsValidIndex(Index))
        {
            InventorySlots[Index]->UpdateItem (-1);
        }
    }
}

void UInventoryWidget::RefreshInventory()
{
    RefreshInventory(-1);
}

void UInventoryWidget::CreateAndAddSlot(int32 Index)
{
    const FInventoryItem& Item = InventoryManager->ItemsArray[Index];
    UInventorySlotWidget* SlotToAdd = CreateWidget<UInventorySlotWidget>(GetWorld(), SlotWidgetClass);

    SlotToAdd->SetItem(Item.Object, Item.Count);
    SlotToAdd->SetConfig(InventoryManager, Index);

    int Row = Index / GridSize;
    int Col = Index % GridSize;

    UGridSlot* GridSlot = ItemGrid->AddChildToGrid(SlotToAdd, Row, Col);
    if (GridSlot)
    {
        GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
        GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
    }

	if (Index >= InventorySlots.Num())
	{
		InventorySlots.SetNum(Index + 1);
	}   
	InventorySlots[Index] = SlotToAdd;
}


