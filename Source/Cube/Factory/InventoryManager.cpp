#include "InventoryManager.h"


UInventoryManager::UInventoryManager()
{
	if (AutoExpanding)
	{
		SetNum(GridSize);
	}
		
}

bool UInventoryManager::AddItem(const FInventoryItem& NewItem, int Position, bool stack)
{
	int size = ItemsArray.Num();
	if (!NewItem.Object) return 0;

	if (Position != -1)
	{
		if (AutoExpanding)
		{
			if (Position + GridSize >= size)
				ItemsArray.SetNum(((int)((Position / GridSize) + 2)) * GridSize);
		}
		else
		{
			if (Position >= size)
				return 0;
		}
		ItemsArray[Position] = NewItem;
	}
	else
	{
		bool IsPlaced = 0;		
		if (stack)
		{
			for (auto& i : ItemsArray)
			{
				if (i.Object == NewItem.Object)
				{
					i.Count += NewItem.Count;
					IsPlaced = 1;
					break;
				}
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
		if (AutoExpanding)
		{
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
		if (!IsPlaced) return 0;
		
	}
	if (AutoExpanding)
	{
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
	}
	OnItemsChanged.Broadcast();

	return 1;
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

void UInventoryManager::SetNum(int Size)
{
	ItemsArray.SetNum(Size);
}

FInventoryItem UInventoryManager::GetItem(int Index)
{
	if (Index >= ItemsArray.Num())
		return FInventoryItem();
	return ItemsArray[Index];

}

TArray<int> UInventoryManager::GetItems(UItemInfo* Object)
{
	TArray<int> FoundItems;
	int counter = 0;
	for (auto& i : ItemsArray)
	{
		if (i.Object == Object)
		{
			FoundItems.Push(counter);			
		}
		++counter;
	}

	return FoundItems;
}


//void UInventoryManager::SwapItems(uint16 PositionA, uint16 PositionB)
//{
//	int32 size = ItemsArray.Num();
//	if (Position >= size) return;
//	Swap();
//}
