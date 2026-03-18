// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayersTool.h"
#include "Cube/Player/HumanController.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "Cube/Fight/Weapons/Weapon.h"

UPlayersTool::UPlayersTool()
{
	
	
}

void UPlayersTool::BeginPlay()
{
	Super::BeginPlay();
	
}


void UPlayersTool::SetHandMode(EHandMode NewMode)
{
	if (TecHandMode == NewMode) return;

	TecHandMode = NewMode;
	CheckItems();

	
}

void UPlayersTool::DoNothing()
{
	if (ObjectOnHand)ObjectOnHand->Destroy();
}

void UPlayersTool::ChangeWeapon()
{
	DoNothing();
	auto weapon = PlayerController->PlayerEquipmentInventory->GetItem(EItemsInInventoryID::weapon);
	if (weapon.Count < 1 ||(! weapon.Object)) return;

	UWorld* World = GetWorld();
	if (!World) return;

	if (weapon.Object->Type == EItemType::weapon && weapon.Object->Object && weapon.Object->Object->IsChildOf<AActor>())
	{
		FTransform SpawnTransform = GetComponentTransform();

		ObjectOnHand = World->SpawnActor<AWeapon>(weapon.Object->Object, SpawnTransform);

		ObjectOnHand->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

		if (auto* weap = Cast <AWeapon>(ObjectOnHand))
		{
			WeaponOnHand = weap;
			WeaponOnHand->SetMyOwner(PlayerController);
		}
	}

	

	
}

void UPlayersTool::SetController(AHumanController* NewController)
{
	PlayerController = NewController;
	PlayerController->PlayerEquipmentInventory->OnItemsChanged.AddUObject(this, &UPlayersTool::CheckItems);
}

void UPlayersTool::CheckItems()
{
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

void UPlayersTool::Weapon_StopAttak()
{
	if (WeaponOnHand)
		WeaponOnHand->StopAttak();
}

void UPlayersTool::Weapon_StartAttak()
{
	if (WeaponOnHand)
		WeaponOnHand->StartAttak();
}

void UPlayersTool::Weapon_SetAttaking(bool Value)
{
	if (WeaponOnHand)
		WeaponOnHand->SetAttaking(Value);
}

void UPlayersTool::Weapon_Attak()
{
	if (WeaponOnHand)
		WeaponOnHand->Attak();
}

