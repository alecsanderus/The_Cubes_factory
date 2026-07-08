#include "InventoryManager.h"


UInventoryManager::UInventoryManager()
{
	if (AutoExpanding)
	{
		SetNum(GridSize);
	}
		
}

/**
 * Добавляет предмет(ы) в инвентарь.
 *
 * Поведение:
 * - Проверяет корректность `NewItem.Object` и `NewItem.Count`.
 * - Если `stack == true`, пытается добавить количество в существующие слоты с тем же объектом,
 *   используя `CanSetItem` для определения допустимого количества в конкретной ячейке.
 * - Если после попытки сложить остались незанятые единицы и `ItemsCanSwitchType == true`,
 *   заполняет пустые слоты (где `Count <= 0`) этим предметом с учётом `CanSetItem`.
 * - Если остаётся `ToAdd > 0` и `AutoExpanding == true`, добавляет новый слот в `ItemsArray`.
 * - Вызывает `CheckExpanding()` для корректировки размера массива при необходимости.
 * - Если `Hide == false`, вызывает событие `OnItemsChanged.Broadcast()`.
 *
 * Параметры:
 * - `NewItem` — добавляемый элемент (объект + количество).
 * - `stack` — разрешать ли укладку в уже существующие стеки одинакового объекта.
 * - `Hide` — подавлять ли вызов события об изменении инвентаря (`OnItemsChanged`).
 *
 * Возвращаемое значение:
 * - Количество единиц, которые не удалось добавить (остаток). 0 означает, что весь `NewItem.Count` размещён.
 *
 * Эффекты:
 * - Модифицирует `ItemsArray` (увеличивает `Count`, может установить `Object` в пустых слотах
 *   или добавить новый слот).
 * - Может вызвать `OnItemsChanged.Broadcast()` (если `Hide == false`). 
 */


int UInventoryManager::AddItem(const FInventoryItem& NewItem, bool stack, bool Hide)
{
	int size = ItemsArray.Num();
	if (!NewItem.Object) return 0;
	if (NewItem.Count <= 0) return 0;
	TArray <int32> UpdatedSlots;
	int ToAdd = NewItem.Count;
	if (stack)
	{
		for (int pos = 0; pos < ItemsArray.Num(); pos++)
		{
			auto& i = ItemsArray[pos];
			if (i.Object == NewItem.Object)
			{
				int can = CanSetItem({ NewItem.Object, i.Count + ToAdd }, pos);
				can -= i.Count;
				if (can > 0)
				{
					ToAdd -= can;
					i.Count += can;
					if (ToAdd == 0)
						break;
					UpdatedSlots.Add(pos);
				}
			}
		}
	}
	if (ToAdd > 0)
	{
		for (int pos = 0; pos < ItemsArray.Num(); pos++)
		{
			auto& i = ItemsArray[pos];
			if (i.Count <= 0)
			{
				int can = CanSetItem({ NewItem.Object, ToAdd }, pos);
				if (can > 0)
				{
					ToAdd -= can;
					i.Count = can;
					i.Object = NewItem.Object;
					if (ToAdd == 0)
						break;
					UpdatedSlots.Add(pos);

				}
			}
		}
	}

	if (ToAdd > 0 && AutoExpanding)
	{
		int can = CanSetItem({ NewItem.Object, ToAdd }, -1);
		ItemsArray.Push({ NewItem.Object, can });
		ToAdd -= can;
		UpdatedSlots.Add(ItemsArray.Num() - 1);
	}

	CheckExpanding();		
	
	if (!Hide)
	{
		for (int i : UpdatedSlots)
		{
			OnItemsChanged.Broadcast(i);
		}
	}

	return ToAdd;
}

int UInventoryManager::CanSetItem(const FInventoryItem& NewItem, int Position)
{
	int size = ItemsArray.Num();

	if (Position >= size)
		return 0;


	if (ItemsCanSwitchType)
	{
		return  CanAddItem(NewItem);
	}
	else
	{
		if (Position == -1)
			return  0;

		if (NewItem.Object == ItemsArray[Position].Object)
			return CanAddItem(NewItem);
		else
			return 0;
	}
}

int UInventoryManager::CanAddItem(const FInventoryItem& NewItem)
{
	// add logic to check if the item can be added to the inventory by weight, size, or other constraints
	return NewItem.Count;
}

bool UInventoryManager::SetItem(const FInventoryItem& NewItem, int Position, bool Hide, bool DoNotCheckExpanding)
{
	if (CanSetItem(NewItem, Position) != NewItem.Count)
		return 0;

	int size = ItemsArray.Num();
	if (Position >= size)
	{
		if (AutoExpanding)
			ItemsArray.SetNum(Position + 1);
		else
			return 0;
	}

	if (ItemsCanBeNull)
		ItemsArray[Position] = NewItem;
	else
	{
		if (!NewItem.Object)
			ItemsArray[Position].Count = 0;
		else
			ItemsArray[Position] = NewItem;
	}	
	if (!(DoNotCheckExpanding || Hide))
		CheckExpanding();

	if (!Hide)
		OnItemsChanged.Broadcast(Position);
	return 1;
}

void UInventoryManager::CheckExpanding()
{
	if (AutoExpanding)
	{
		int size = ItemsArray.Num();
		int LastElPosition = 0;
		for (int i = size - 1; i > 0; --i)
		{
			if (ItemsArray[i].Object)
			{
				LastElPosition = i;
				break;
			}
		}
		int NewSize = ((int)((LastElPosition) / GridSize)) * GridSize + GridSize * 2;
		if (NewSize != size)
		{
			ItemsArray.SetNum(NewSize);
			OnItemsChanged.Broadcast(-2);
		}
	}
}


void UInventoryManager::SetNum(int Size)
{
	ItemsArray.SetNum(Size);
}

void UInventoryManager::CheckInventory()
{
	OnItemsChanged.Broadcast(-1);
}

void UInventoryManager::SetItemOnSlot(const FInventoryItem& NewItem, int Position, bool Hide)
{
	if (Position >= ItemsArray.Num()) return;
	ItemsArray[Position] = NewItem;
	if (!Hide)
		OnItemsChanged.Broadcast(Position);
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
