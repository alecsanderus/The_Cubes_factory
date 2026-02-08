// Fill out your copyright notice in the Description page of Project Settings.

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemsChanged);


UCLASS()
class CUBE_API UInventoryManager : public UObject
{
	GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 GridSize = 13;
public:

    UInventoryManager();



    UPROPERTY(EditAnywhere)
    bool AutoExpanding = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray <FInventoryItem> ItemsArray;
	
    UFUNCTION(BlueprintCallable)
    bool AddItem(const FInventoryItem& NewItem, int Position = -1, bool stack = true);

    UFUNCTION(BlueprintCallable)
    void RemoveItem(int Position);

    UFUNCTION(BlueprintCallable)
    void SetNum(int Size);



    FInventoryItem GetItem(int Index);
    TArray <int> GetItems(UItemInfo* Object);

    UPROPERTY(BlueprintAssignable)
    FOnItemsChanged OnItemsChanged;
    //void SwapItems(uint16 PositionA, uint16 PositionB);
};
