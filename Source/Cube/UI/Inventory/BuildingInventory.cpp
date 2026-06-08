#include "BuildingInventory.h"
#include "InventorySlotWidget.h"

void UBuildingInventory::SetInventoryManager(UInventoryManager* Manager)
{
	for (int el = 0; el < Slots.Num(); el++)	
	{
		Slots[el]->SetConfig(Manager, 1);
	}
}
