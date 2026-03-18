

#include "Components/Button.h"
#include "GigaButton.h"
#include "Cube/DebugMacros.h"

void UGigaButton::SetId(int ID){MyId = ID;}



void UGigaButton::SetButtonImage(UTexture2D* Texture, float SizeX, float SizeY)
{
    if (!Texture) return;
    DEBUG_CHECK_RETURN("UGigaButton", "MainButton", MainButton);

    FSlateBrush NormalBrush; 
    NormalBrush.SetResourceObject(Texture);
    NormalBrush.DrawAs = ESlateBrushDrawType::Image;

    if (SizeX != -1 && SizeY != -1)
        NormalBrush.ImageSize = FVector2D(SizeX, SizeY);


    FSlateBrush HoveredBrush = NormalBrush;
    FSlateBrush PressedBrush = NormalBrush;


    HoveredBrush.TintColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.f));
    PressedBrush.TintColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));


   

    FButtonStyle ButtonStyle;
    ButtonStyle.SetNormal(NormalBrush);
    ButtonStyle.SetHovered(HoveredBrush);
    ButtonStyle.SetPressed(PressedBrush);
    ButtonStyle.SetDisabled(NormalBrush);

    MainButton->SetStyle(ButtonStyle);
}


void UGigaButton::NativeConstruct() {
    Super::NativeConstruct();
    DEBUG_CHECK_RETURN("UGigaButton", "MainButton", MainButton);

    MainButton->OnClicked.Clear();
    MainButton->OnPressed.Clear();
    MainButton->OnReleased.Clear();
    MainButton->OnHovered.Clear();
    MainButton->OnUnhovered.Clear();

    MainButton->OnClicked.AddDynamic(this, &UGigaButton::OnClickedHandler);
    MainButton->OnPressed.AddDynamic(this, &UGigaButton::OnPressedHandler);
    MainButton->OnReleased.AddDynamic(this, &UGigaButton::OnReleasedHandler);
    MainButton->OnHovered.AddDynamic(this, &UGigaButton::OnHoveredHandler);
    MainButton->OnUnhovered.AddDynamic(this, &UGigaButton::OnUnhoveredHandler);
}
void UGigaButton::OnClickedHandler()    { OnButtonTriggered.Broadcast(MyId, FButtonTriggeredType::Clicked); }
void UGigaButton::OnPressedHandler()    { OnButtonTriggered.Broadcast(MyId, FButtonTriggeredType::Pressed); }
void UGigaButton::OnReleasedHandler()   { OnButtonTriggered.Broadcast(MyId, FButtonTriggeredType::Released); }
void UGigaButton::OnHoveredHandler()    { OnButtonTriggered.Broadcast(MyId, FButtonTriggeredType::Hovered); }
void UGigaButton::OnUnhoveredHandler()  { OnButtonTriggered.Broadcast(MyId, FButtonTriggeredType::Unhovered); }
