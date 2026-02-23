// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Cube/Player/HumanController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Привязка функций к кнопкам
    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnResumeClicked);
    }

    if (QuitButtonToMenu)
    {
        QuitButtonToMenu->OnClicked.AddDynamic(this, &UPauseWidget::OnQuitMenuClicked);
    }

    if (QuitButtonToDesctrop)
    {
        QuitButtonToDesctrop->OnClicked.AddDynamic(this, &UPauseWidget::OnQuitDesctropClicked);
    }
}

void UPauseWidget::OnResumeClicked()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (auto hc = Cast <AHumanController>(PC))
            hc->SwitchPause();
    }
}

void UPauseWidget::OnQuitMenuClicked()
{
    UGameplayStatics::OpenLevel(this, FName("/Game/Maps/MainMenu/MainMenu"));
}

void UPauseWidget::OnQuitDesctropClicked()
{
    //UKismetSystemLibrary::QuitGame()
    UKismetSystemLibrary::QuitGame(GetWorld(), Cast <APlayerController>(GetWorld()->GetFirstPlayerController()), EQuitPreference::Quit, true);
}