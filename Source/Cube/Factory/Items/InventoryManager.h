#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemInfo.h"
#include "InventoryManager.generated.h"



USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemInfo* Object = nullptr;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", UIMin = "1"))
    int Count = -1;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemsChanged, int32);


UCLASS()
class CUBE_API UInventoryManager : public UObject
{
	GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 GridSize = 13;
public:

    UInventoryManager();

    bool ItemsCanSwitchType = 1;
    bool ItemsCanBeNull = 1;

    UPROPERTY(EditAnywhere)
    bool AutoExpanding = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray <FInventoryItem> ItemsArray;
	
    UFUNCTION(BlueprintCallable)
    int AddItem(const FInventoryItem& NewItem, bool stack = true, bool Hide = false);

    UFUNCTION(BlueprintCallable)
    int CanSetItem(const FInventoryItem& NewItem, int Position);

    UFUNCTION(BlueprintCallable)
    int CanAddItem(const FInventoryItem& NewItem);

    UFUNCTION(BlueprintCallable)
    bool SetItem(const FInventoryItem& NewItem, int Position, bool Hide = false, bool DoNotCheckExpanding = false);

    void CheckExpanding();


    UFUNCTION(BlueprintCallable)
    void SetNum(int Size);

    UFUNCTION(BlueprintCallable)
    void CheckInventory();

    UFUNCTION(BlueprintCallable)
    void SetItemOnSlot(const FInventoryItem& NewItem, int Position, bool Hide = false);
	
    FInventoryItem GetItem(int Index);
    TArray <int> GetItems(UItemInfo* Object);

	FOnItemsChanged OnItemsChanged; // -1 -> изменились ВСЕ, -2 -> изменился только размер массива, иначе индекс изменённого слота


    //void SwapItems(uint16 PositionA, uint16 PositionB);
};
