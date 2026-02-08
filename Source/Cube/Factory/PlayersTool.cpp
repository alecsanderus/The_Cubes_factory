// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayersTool.h"
#include "Cube/HumanController.h"
#include "InventoryManager.h"

APlayersTool::APlayersTool()
{
	PrimaryActorTick.bCanEverTick = true;
	MainComp = CreateDefaultSubobject <USceneComponent>(TEXT("MainComp"));
	RootComponent = MainComp;
}

void APlayersTool::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayersTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void APlayersTool::SetHandMode(EHandMode NewMode)
{
	if (TecHandMode == NewMode) return;

	TecHandMode = NewMode;
	

	switch (TecHandMode)
	{
	case Nothing:
		DoNothing();

		break;
	case HandlingWeapon:
		ChangeWeapon();

		break;
	case Building:


		break;
	case Destroying:



		break;
	default:
		break;
	}
}

void APlayersTool::DoNothing()
{
	if (ObjectOnHand)ObjectOnHand->Destroy();
}

void APlayersTool::ChangeWeapon()
{
	DoNothing();
	auto weapon = PlayerController->PlayerEquipmentInventory->GetItem(EItemsInInventoryID::weapon);
	if (weapon.Count < 1) return;

	
}

void APlayersTool::SetController(AHumanController* NewController)
{
	PlayerController = NewController;
}

