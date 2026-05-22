#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingConfig.generated.h"


USTRUCT(BlueprintType)
struct FMaterialsToBuild
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UItemInfo* Item;

	UPROPERTY(EditAnywhere)
	int col = 1;
};


UCLASS()
class CUBE_API UBuildingConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* PrevewMesh;
	
	UPROPERTY(EditAnywhere)
	TArray <FMaterialsToBuild> Materials;
		 
	UPROPERTY(EditAnywhere)
	TSubclassOf <AActor> Object;


	UPROPERTY(EditAnywhere)
	FText Description;


	UPROPERTY(EditAnywhere)
	class UBuildingConfigCategory* Category;

	UPROPERTY(EditAnywhere)
	int Priority;


	
};
