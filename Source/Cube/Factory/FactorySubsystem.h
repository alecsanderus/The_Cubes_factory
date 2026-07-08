#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "FactorySubsystem.generated.h"


USTRUCT()
struct FItemPair
{
	GENERATED_BODY();

	UPROPERTY()
	FPrimaryAssetId ID;

	UPROPERTY()
	int Num;
};



USTRUCT()
struct FRecipe
{
	GENERATED_BODY();

	UPROPERTY()
	FGuid RecipeID;

	UPROPERTY()
	TArray <FItemPair> Inputs;

	UPROPERTY()
	TArray <FItemPair> Outputs;

	UPROPERTY()
	int TicksToCreate = 1;

	UPROPERTY()
	FName Name;

};

USTRUCT()
struct FMachine
{
	GENERATED_BODY();

	UPROPERTY()
	FGuid MachineID;

	UPROPERTY()
	FGuid RecipeID;

	UPROPERTY()
	TSubclassOf <AActor> MashineClass;

	UPROPERTY()
	FTransform MachineTranform;

	UPROPERTY()
	int Progress = -1;

	UPROPERTY()
	TArray <FItemPair> Inputs;

	UPROPERTY()
	TArray <FItemPair> Outputs;
};


DECLARE_MULTICAST_DELEGATE(FOnItemsChangedOnSubsystem)


UCLASS()
class CUBE_API UFactorySubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual TStatId GetStatId() const override;


	UPROPERTY(EditAnywhere)
	TArray <FMachine> MachinesList;

	UPROPERTY()
	TMap <FGuid, int> MachineLookupMap;


	UPROPERTY(EditAnywhere)
	TArray <FRecipe> RecipesList;

	UPROPERTY(EditAnywhere)
	TMap <FGuid, int> RecipeLookupMap;

	UPROPERTY(EditAnywhere)
	TMap <FName, FGuid> RecipesNamesMap;

	void RegisterMachine(FMachine Machine);

	void RegisterRecipe(FRecipe Recipe);


	FMachine* GetMachine(int number);
	FMachine* GetMachine(FGuid Guid);

	FRecipe* GetRecipe(int number);
	FRecipe* GetRecipe(FGuid Guid);
	FRecipe* GetRecipe(FName Name);

	FOnItemsChangedOnSubsystem OnItemsChanged;
};
