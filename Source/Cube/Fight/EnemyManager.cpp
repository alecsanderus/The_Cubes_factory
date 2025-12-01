// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EnemySpawnPoint.h"
#include "algorithm"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

int AEnemyManager::FindNextByWeight(std::vector <int> &arr)
{	
	int  i = rand() % arr[arr.size()-1];


	auto it = std::upper_bound(arr.begin(), arr.end(), i,
		[](int value, int element) {
			return value < element;
		});

	if (it != arr.end())
		return it - arr.begin();
	else
		return 0;
	
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	if (Spawns.Num() <= 0 || Enemys.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyManager has no Spawns or Enemys!    name: %s"), *this->GetName());		
		return;
	}

	SpawnWeights.assign(Spawns.Num(), 0);	
	SpawnWeights[0] = Spawns[0].weight;
	for (int i = 1; i < Spawns.Num(); ++i)
	{
		SpawnWeights[i] = Spawns[i].weight + SpawnWeights[i-1];
	}

	EnemWeights.assign(Enemys.Num(), 0);
	EnemWeights[0] = Enemys[0].weight;
	for (int i = 1; i < Enemys.Num(); ++i)
	{
		EnemWeights[i] = Enemys[i].weight + EnemWeights[i - 1];
	}
	NextID = FindNextByWeight(EnemWeights);
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Spawns.Num() <= 0 || Enemys.Num() <= 0)
	{		
		return;
	}
	Money += DeltaTime;
	if (Enemys[NextID].cost <= Money)
	{
		Money -= Enemys[NextID].cost;		
		Spawns[FindNextByWeight(SpawnWeights)].SpawnInstance->SpawnEnemy(Enemys[NextID].EnemyClass);
		NextID = FindNextByWeight(EnemWeights);		
	}

}

