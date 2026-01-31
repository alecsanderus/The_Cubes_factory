#include "InventoryManager.h"


void UInventoryManager::AddItem (const FInventoryItem& NewItem, int Position = -1)
{
	int size = ItemsArray.Num();
	if (NewItem.Object == nullptr) return;

	if (Position != -1)
	{
		if (Position + GridSize >= size)
			ItemsArray.SetNum(((int)((Position / GridSize) + 2)) * GridSize);
		ItemsArray[Position] = NewItem;
	}
	else
	{
		bool IsPlaced = 0;		
		for (auto& i : ItemsArray)
		{
			if (i.Object == NewItem.Object)
			{
				i.Count += NewItem.Count;
				IsPlaced = 1;
				break;
			}
		}
		if (!IsPlaced)
		{
			int counter = 0;
			for (auto& i : ItemsArray)
			{
				if (i.Count == -1)
				{
					i = NewItem;
					IsPlaced = 1;
					Position = counter;
					break;
				}
				++counter;
			}
		}

		if (!IsPlaced)
		{
			if (size + GridSize >= size)
				ItemsArray.SetNum(((int)((size / GridSize) + 2)) * GridSize);
			ItemsArray[size] = NewItem;
		}
		else
		if (Position + GridSize >= size)
			ItemsArray.SetNum(((int)((Position / GridSize) + 2)) * GridSize);
		
	}
	size = ItemsArray.Num();
	int NewSiz = 0;
	for (int i = size - 1; i > 0; --i)
	{
		if (ItemsArray[i].Count != -1)
		{
			NewSiz = i;
			break;
		}
	}
	if ((((int)((NewSiz + GridSize) / GridSize)) * GridSize) < size)
		ItemsArray.SetNum(((int)((NewSiz / GridSize) + 2)) * GridSize);
	OnItemsChanged.Broadcast();
}

void UInventoryManager::RemoveItem(int Position)
{
	int size = ItemsArray.Num();
	if (Position >= size) return;
	ItemsArray[Position] = FInventoryItem();
	int NewSiz = 0;
	for (int i = size - 1; i > 0; --i)
	{
		if (ItemsArray[i].Count != -1)
		{
			NewSiz = i;			
			break;
		}
	}
	if ((((int)((NewSiz + GridSize) / GridSize)) * GridSize) < size)
		ItemsArray.SetNum(((int)((NewSiz / GridSize) + 2)) * GridSize);
	OnItemsChanged.Broadcast();
}

//void UInventoryManager::SwapItems(uint16 PositionA, uint16 PositionB)
//{
//	int32 size = ItemsArray.Num();
//	if (Position >= size) return;
//	Swap();
//}
