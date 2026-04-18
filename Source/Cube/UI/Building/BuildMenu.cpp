#include "BuildMenu.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Cube/Factory/Building/BuildingConfig.h"
#include "Cube/Factory/Building/BuildingConfigCategory.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Cube/DebugMacros.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "Cube/Player/PlayerChar.h"
#include "Cube/Player/PlayersTool.h"
#include "Cube/Player/HumanController.h"



void UBuildMenu::BuildMenu()
{
    GetGameInstance();
    DEBUG_CHECK_RETURN("UBuildMenu", CategoryButtonClass);
    DEBUG_CHECK_RETURN("UBuildMenu", BuildingButtonClass);
    
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    FARFilter Filter;
    Filter.PackagePaths.Add(FName("/Game/Buildings/DataOnMenu"));
    Filter.ClassPaths.Add(FTopLevelAssetPath(UBuildingConfig::StaticClass()));
    Filter.bRecursivePaths = true;

    TArray<FAssetData> AssetDataList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

    UE_LOG(LogTemp, Warning, TEXT("/Game/Buildings/DataOnMenu     %d"), AssetDataList.Num());
    Configs.Reserve(AssetDataList.Num());
    for (const FAssetData& AssetData : AssetDataList)
    {
        UObject* AssetObject = AssetData.GetAsset();
        if (UBuildingConfig* DataAsset = Cast<UBuildingConfig>(AssetObject))
        {
            UE_LOG(LogTemp, Warning, TEXT("Asset: %s"), *DataAsset->GetName());
            Configs.Push(DataAsset);
        }
    }



    Configs.Sort([](const UBuildingConfig& a, const UBuildingConfig& b) {
        if (a.Category != nullptr && b.Category != nullptr && a.Category != b.Category) return a.Category->Priority < b.Category->Priority;
        if (a.Category != nullptr && b.Category == nullptr) return true;
        if (a.Category == nullptr && b.Category != nullptr) return false;

        return a.Priority < b.Priority;
        });


    {      
        auto TecConf = Configs.CreateConstIterator();
        int TecIDCat = 0, TecIDConf = 0;
        while (TecConf)
        {
            auto TecCat = (*TecConf)->Category;

            UGigaButton* Button = NewObject<UGigaButton>(this, CategoryButtonClass);

            auto ElInBox = CategoriesBox->AddChildToVerticalBox(Button);
            ElInBox->SetHorizontalAlignment(HAlign_Center);
            ElInBox->SetVerticalAlignment(VAlign_Center);

            CaterogyButtons.Add(Button);
            Button->SetId(TecIDCat);
            if (TecCat != nullptr)
                Button->SetButtonImage(TecCat->Icon);

            Button->OnButtonTriggered.AddDynamic(this, &UBuildMenu::OnCategorySwitched);
            TecIDCat++;





            UScrollBox* ScrollBox = NewObject<UScrollBox>(this);
            CategorySwitcher->AddChild(ScrollBox);
         

            UWrapBox* WrapBox = NewObject<UWrapBox>(this);
            ScrollBox->AddChild(WrapBox);
                   
            while (TecConf && (*TecConf)->Category == TecCat)
            {
                UGigaButton* NewButton = CreateWidget<UGigaButton>(this, BuildingButtonClass);
                if (NewButton)
                {
                    WrapBox->AddChild(NewButton);              

                    NewButton->SetButtonImage((*TecConf)->Icon);
                    NewButton->SetId(TecIDConf);
                    NewButton->OnButtonTriggered.AddDynamic(this, &UBuildMenu::OnBuildinTapTapTap);

                }
                TecConf++;
                TecIDConf++;
            }          
        }



    }

}

void UBuildMenu::OnCategorySwitched(int32 Id, FButtonTriggeredType Type)
{
    if (Type == FButtonTriggeredType::Clicked)
        CategorySwitcher->SetActiveWidgetIndex(Id);
}

void UBuildMenu::OnBuildinTapTapTap(int32 Id, FButtonTriggeredType Type)
{
    switch (Type)
    {
    case FButtonTriggeredType::Clicked:
    {
        auto paw = GetOwningPlayerPawn();
        auto player = Cast <APlayerChar>(paw);
        if (player)
        {
            player->PlayerTool->SetHandMode(EHandMode::Building, Configs[Id]);
            if (auto contr = Cast <AHumanController> (player->GetController()))
                contr->SwitchBuildingMenu();
        }


    }
        break;
    case FButtonTriggeredType::Pressed:
        break;
    case FButtonTriggeredType::Released:
        break;
    case FButtonTriggeredType::Hovered:
    {
        DescriptionText->SetText(FText::FromString(Configs[Id]->Description));
        DescriptionImage->SetBrushFromTexture(Configs[Id]->Icon);
    }

        break;
    case FButtonTriggeredType::Unhovered:
        break;
    default:
        break;
    }
}

