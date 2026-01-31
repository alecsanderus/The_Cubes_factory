// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGameInstance.h"
#include "Cube/Factory/InventoryManager.h"
#include "Kismet/GameplayStatics.h"
#include "CubeSaveGame.h"

void UCubeGameInstance::Init()
{
	PlayerInventory = NewObject<UInventoryManager>(this, UInventoryManager::StaticClass());    
}


void UCubeGameInstance::SaveGame()
{
    
    UCubeSaveGame* SaveGameObject =
        Cast<UCubeSaveGame>(UGameplayStatics::CreateSaveGameObject(UCubeSaveGame::StaticClass()));

    
    SaveGameObject->ItemsArray = PlayerInventory->ItemsArray;

    UGameplayStatics::SaveGameToSlot(SaveGameObject, TEXT("CubeGameSave"), 0);
}

void UCubeGameInstance::LoadGame()
{
    if (UCubeSaveGame* Loaded =
        Cast<UCubeSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("CubeGameSave"), 0)))
    {
      
        PlayerInventory->ItemsArray = Loaded->ItemsArray;
    }
}
