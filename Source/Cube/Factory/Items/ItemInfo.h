#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemInfo.generated.h"

UENUM()
enum class EItemType : uint8
{
	nothing,
	resource,
	equipment,
	weapon
};







UCLASS()
class CUBE_API UItemInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	EItemType Type = EItemType::nothing;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr <UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr <UTexture2D> Icon;

	UPROPERTY(EditAnywhere)
	int32  Weight = 1;

	
	UPROPERTY(EditAnywhere)
	TSubclassOf <UObject> Object;


	UPROPERTY(EditAnywhere)
	FText Description;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Item", GetFName());
	}


//#if WITH_EDITOR
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
//	{
//		Super::PostEditChangeProperty(PropertyChangedEvent);
//
//		if (!RecipeID.IsValid())
//		{
//			RecipeID = FGuid::NewGuid();
//			MarkPackageDirty();
//		}
//	}
//#endif
};
