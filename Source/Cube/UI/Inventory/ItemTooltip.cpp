#include "ItemTooltip.h"
#include "Cube/Factory/Items/ItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemTooltip::SetItem(UItemInfo* Info)
{
	if (!Info) return;

    if (Info->Icon)
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(Info->Icon);
        Icon->SetBrush(Brush);
    }


	Name->SetText(FText::FromString (Info->Name));
    Description->SetText(FText::FromString(Info->Description));
    Weight->SetText(FText::AsNumber(Info->Weight));
}

