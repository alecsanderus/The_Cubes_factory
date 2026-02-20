#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltip.generated.h"

class UTextBlock;


UCLASS()
class CUBE_API UItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Weight;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

	UFUNCTION (BlueprintCallable)
	void SetItem(class UItemInfo* Info);

	
};
