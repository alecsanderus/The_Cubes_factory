// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class CUBE_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // Привязанные функции для кнопок
    UFUNCTION(BlueprintCallable)
    void OnResumeClicked();

    UFUNCTION(BlueprintCallable)
    void OnQuitMenuClicked();

    UFUNCTION(BlueprintCallable)
    void OnQuitDesctropClicked();

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButtonToMenu;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButtonToDesctrop;

    virtual void NativeConstruct() override;
};
