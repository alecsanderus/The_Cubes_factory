#include "BuildingInventory.h"
#include "InventorySlotWidget.h"
#include "Blueprint/WidgetTree.h"

void UBuildingInventory::SetInventoryManager(UInventoryManager* Manager)
{
	for (int el = 0; el < SlotsIn.Num(); el++)
	{
        SlotsIn[el]->SetConfig(Manager, el);
	}

    for (int el = 0; el < SlotsOut.Num(); el++)
    {
        SlotsOut[el]->SetConfig(Manager, el + SlotsIn.Num());
    }
}


void UBuildingInventory::NativeConstruct()
{
    Super::NativeConstruct();

    auto InitializeSlots = [this](const TArray<FName>& SlotNames, TArray<UInventorySlotWidget*>& Slots)
        {
            Slots.Empty();

            for (const FName& Name : SlotNames)
            {
                if (UWidget* FoundWidget = WidgetTree->FindWidget(Name))
                {
                    if (UInventorySlotWidget* CastedSlot = Cast<UInventorySlotWidget>(FoundWidget))
                    {
                        Slots.Add(CastedSlot);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("BOBR CURVA Widget with name [%s] found, but it is not a UInventorySlotWidget!"), *Name.ToString());
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("BOBR CURVA Could not find widget with name [%s] in the Widget Tree!"), *Name.ToString());
                }
            }
        };
    InitializeSlots(SlotInNames, SlotsIn);
    InitializeSlots(SlotOutNames, SlotsOut);
}