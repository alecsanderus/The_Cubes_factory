// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGhost.h"

// Sets default values
ABuildingGhost::ABuildingGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingGhost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

