#include "CubeGameInstance.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Cube/Player/CubeSaveGame.h"

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
