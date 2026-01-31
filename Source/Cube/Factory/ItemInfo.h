// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemInfo.generated.h"


UCLASS()
class CUBE_API UItemInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	FString Name = "Nou Name Kakoy-to";


	UPROPERTY(EditAnywhere)
	FString Deskription = "Umniy Chelovec";

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere)
	int Weight = 1;

	
	UPROPERTY(EditAnywhere)
	TSubclassOf <UObject> Object;
};
