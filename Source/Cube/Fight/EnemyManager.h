// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vector"
#include "EnemyManager.generated.h"

class AEnemySpawnPoint;




USTRUCT()
struct FEnemyType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	unsigned int cost;

	UPROPERTY(EditAnywhere)
	unsigned int weight;

	UPROPERTY(EditAnywhere)
	TSubclassOf <AActor> EnemyClass;

};

USTRUCT()
struct FSpawns
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere)
	unsigned int weight;

	UPROPERTY(EditAnywhere)
	AEnemySpawnPoint* SpawnInstance;

};


UCLASS()
class CUBE_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();


	int FindNextByWeight(std::vector <int>& arr);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY (EditAnywhere)
	TArray <FSpawns> Spawns;
	std::vector <int> SpawnWeights;

	UPROPERTY(EditAnywhere)
	TArray <FEnemyType> Enemys;
	std::vector <int> EnemWeights;

	float Money = 0;

	unsigned int NextID = 0;
	unsigned int SpawnNextID = 0;
	unsigned int Cost = 0;
};
