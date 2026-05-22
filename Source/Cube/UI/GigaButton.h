#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GigaButton.generated.h"

UENUM()
enum class FButtonTriggeredType
{
	Clicked,
	Pressed,
	Released,
	Hovered,
	Unhovered

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonTriggered, int32, Id, FButtonTriggeredType, Type);
//DECLARE_DELEGATE_TwoParam(FOnButtonTriggered, int32, FButtonTriggeredType);

UCLASS()
class CUBE_API UGigaButton : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonTriggered OnButtonTriggered;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainButton;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBitbox;

	void SetId(int ID);
	int MyId = 0;
	void SetButtonImage(UTexture2D* Texture, float SizeX = -1, float SizeY = -1);


protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedHandler();

	UFUNCTION()
	void OnPressedHandler();

	UFUNCTION()
	void OnReleasedHandler();

	UFUNCTION()
	void OnHoveredHandler();

	UFUNCTION()
	void OnUnhoveredHandler();
};
