

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingConfigCategory.generated.h"


UCLASS()
class CUBE_API UBuildingConfigCategory : public UDataAsset
{
	GENERATED_BODY()
public:

	

	UPROPERTY (EditAnywhere)
	FString Name = "Nou name:)";

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	int Priority = 0;

};
