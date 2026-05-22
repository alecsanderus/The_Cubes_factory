#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cube/UI/GigaButton.h"

#include "BuildMenu.generated.h"



UCLASS()
class CUBE_API UBuildMenu : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* CategorySwitcher;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CategoriesBox;

	UPROPERTY()
	TArray <class UGigaButton*> CaterogyButtons;

	UPROPERTY ()
	TArray<class UBuildingConfig*> Configs;

	UPROPERTY()
	TArray <class UBuildingConfigCategory*> Categories;

	UFUNCTION(BlueprintCallable)
	void BuildMenu();

	UFUNCTION(BlueprintCallable)
	void OnCategorySwitched (int32 Id, FButtonTriggeredType Type);

	UFUNCTION(BlueprintCallable)
	void OnBuildinTapTapTap(int32 Id, FButtonTriggeredType Type);


	UPROPERTY(EditAnywhere)
	TSubclassOf <class UGigaButton> CategoryButtonClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf <class UGigaButton> BuildingButtonClass;



	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidget))
	class UImage* DescriptionImage;

	
};
