#include "BuildingInventory.h"
#include "InventorySlotWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Cube/Factory/FactorySubsystem.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "Cube/DebugMacros.h"
#include "Engine/AssetManager.h"


void UBuildingInventory::UpdateInventory()
{
    IsUpdatingNow = 1;
    if (InventoryManager && MyMachineId.IsValid())
    {
        auto subsyst = GetWorld()->GetSubsystem <UFactorySubsystem>();
        auto* mach = subsyst->GetMachine(MyMachineId);


        DEBUG_CHECK_RETURN(UBuildingInventory, mach);

        DEBUG_CHECK_RETURN(UBuildingInventory, (mach->Inputs.Num() == SlotsIn.Num() && mach->Outputs.Num() == SlotsOut.Num()));

        UAssetManager& AssetManager = UAssetManager::Get();

        for (int el = 0; el < SlotsIn.Num(); el++)
        {
            UItemInfo* NewAsset = AssetManager.GetPrimaryAssetObject<UItemInfo>(mach->Inputs[el].ID);
            if (!NewAsset)
            {
                TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAsset(mach->Inputs[el].ID);
                if (Handle.IsValid())
                {
                    Handle->WaitUntilComplete();
                    NewAsset = AssetManager.GetPrimaryAssetObject<UItemInfo>(mach->Inputs[el].ID);
                }
            }
            DEBUG_CHECK(UBuildingInventory, NewAsset)
            {
                InventoryManager->SetItemOnSlot({ NewAsset,  mach->Inputs[el].Num }, el, true);
                //  SlotsIn[el]->SetItem(NewAsset, mach->Inputs[el].Num);
            }
        }

        for (int el = 0; el < SlotsOut.Num(); el++)
        {
            UItemInfo* NewAsset = AssetManager.GetPrimaryAssetObject<UItemInfo>(mach->Outputs[el].ID);
            if (!NewAsset)
            {
                TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAsset(mach->Outputs[el].ID);
                if (Handle.IsValid())
                {
                    Handle->WaitUntilComplete();
                    NewAsset = AssetManager.GetPrimaryAssetObject<UItemInfo>(mach->Outputs[el].ID);
                }
            }
            DEBUG_CHECK(UBuildingInventory, NewAsset)
            {
                InventoryManager->SetItemOnSlot({ NewAsset,  mach->Outputs[el].Num }, el + SlotsIn.Num(), true);

                //     SlotsOut[el]->SetItem(NewAsset, mach->Outputs[el].Num);
            }
        }
        InventoryManager->OnItemsChanged.Broadcast(-1);
    }
    IsUpdatingNow = 0;
}

void UBuildingInventory::SetInventoryManager(FGuid MachineId)
{
	auto subsyst = GetWorld()->GetSubsystem <UFactorySubsystem>();
    auto* mach = subsyst->GetMachine(MachineId);
    

    DEBUG_CHECK_RETURN(UBuildingInventory, mach);

    DEBUG_CHECK_RETURN(UBuildingInventory, (mach->Inputs.Num() == SlotsIn.Num() && mach->Outputs.Num() == SlotsOut.Num()));
    

	MyMachineId = MachineId;
    if (InventoryManager) InventoryManager->MarkAsGarbage();

    InventoryManager = NewObject <UInventoryManager>();
    InventoryManager->AutoExpanding = false;
    InventoryManager->SetNum(SlotsIn.Num() + SlotsOut.Num());

    InventoryManager->OnItemsChanged.AddUObject(this, &UBuildingInventory::ItemsChanged);

	for (int el = 0; el < SlotsIn.Num(); el++)
	{
        SlotsIn[el]->SetConfig(InventoryManager, el, true);
        SlotsIn[el]->EnableAutoUpdate();
	}

    for (int el = 0; el < SlotsOut.Num(); el++)
    {
        SlotsOut[el]->SetConfig(InventoryManager, el + SlotsIn.Num(), true);
        SlotsOut[el]->EnableAutoUpdate();

    }
    subsyst->OnItemsChanged.AddUObject(this, &UBuildingInventory::UpdateInventory);    
}


void UBuildingInventory::NativeConstruct()
{
    Super::NativeConstruct();

    auto InitializeSlots = [this](const TArray<FName>& SlotNames, TArray<UInventorySlotWidget*>& InventorySlots)
        {
            InventorySlots.Empty();

            for (const FName& Name : SlotNames)
            {
                if (UWidget* FoundWidget = WidgetTree->FindWidget(Name))
                {
                    if (UInventorySlotWidget* CastedSlot = Cast<UInventorySlotWidget>(FoundWidget))
                    {
                        InventorySlots.Add(CastedSlot);
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

void UBuildingInventory::ItemsChanged(int32 Index)
{
	if (IsUpdatingNow) return;
    UE_LOG(LogTemp, Warning, TEXT("item change"));

    if (InventoryManager && MyMachineId.IsValid())
    {
        auto subsyst = GetWorld()->GetSubsystem <UFactorySubsystem>();
        auto* mach = subsyst->GetMachine(MyMachineId);


        DEBUG_CHECK_RETURN(UBuildingInventory, mach);

        DEBUG_CHECK_RETURN(UBuildingInventory, (mach->Inputs.Num() == SlotsIn.Num() && mach->Outputs.Num() == SlotsOut.Num()));


        for (int el = 0; el < SlotsIn.Num(); el++)
        {           
            auto item = InventoryManager->GetItem(el);
            auto elem = item.Object;
            FPrimaryAssetId id = elem ? elem->GetPrimaryAssetId() : mach->Inputs[el].ID;
            mach->Inputs[el] = { id, item.Count };
        }

        for (int el = 0; el < SlotsOut.Num(); el++)
        {           
            auto item = InventoryManager->GetItem(el + SlotsIn.Num());
            auto elem = item.Object;
			FPrimaryAssetId id = elem ? elem->GetPrimaryAssetId() : mach->Outputs[el].ID;
            mach->Outputs[el] = {id, item.Count };
        }
    }
}
