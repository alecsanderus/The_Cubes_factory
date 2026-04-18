#include "PlayersTool.h"
#include "Cube/Player/HumanController.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "Cube/Fight/Weapons/Weapon.h"
#include "Cube/Factory/Building/BuildingGhost.h"
#include "Cube/Factory/Building/BuildingConfig.h"
#include "Cube/DebugMacros.h"
#include "Camera/CameraComponent.h"
UPlayersTool::UPlayersTool()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);	
}

void UPlayersTool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Ghost)
	{
		DEBUG_CHECK_RETURN("UPlayersTool", PlayerCamera);
		FHitResult Hit;
		FVector TraceStart = PlayerCamera->GetComponentLocation(); ;
		FVector TraceEnd = TraceStart + PlayerCamera->GetForwardVector() * BuildingMaxDistance;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		Params.AddIgnoredActor(Ghost);


		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			Ghost->SetActorLocation(Hit.Location + FVector (0,0,0.01));
		}
		else
			Ghost->SetActorLocation(TraceEnd);
	}

}

void UPlayersTool::BeginPlay()
{
	Super::BeginPlay();
	
}


void UPlayersTool::SetHandMode(EHandMode NewMode, UObject* param)
{
	if (NewMode == TurnBack) NewMode = OldTecHandMode;
	TecHandMode = NewMode;

	DeleteItemFromHand();

	switch (TecHandMode)
	{
	case Nothing:
	{
		DeleteItemFromHand();
		OldTecHandMode = NewMode;
	}
		break;
	case HandlingWeapon:
	{
		CheckItemsOnHand();
		OldTecHandMode = NewMode;
	}
		break;
	case Building:
	{
		UBuildingConfig* conf;
		if ((!param) || (!(conf = Cast <UBuildingConfig> (param)))) {			
				SetHandMode(TurnBack);			
			break;
		}		

		DEBUG_CHECK_RETURN("UPlayersTool", BuildingGhostClass);
		DEBUG_CHECK_RETURN("UPlayersTool", conf);
		DEBUG_CHECK_RETURN("UPlayersTool", conf->PrevewMesh);

		DeleteItemFromHand();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Ghost = GetWorld()->SpawnActor<ABuildingGhost>(BuildingGhostClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		Ghost->MainMesh->SetStaticMesh(conf->PrevewMesh);
		Ghost->SetColor(0);


	}
		break;
	case Destroying:
		break;
	default:
		break;
	}

	

	
}

void UPlayersTool::DeleteItemFromHand()
{
	if (ObjectOnHand) {
		ObjectOnHand->Destroy(); ObjectOnHand = nullptr;
	}
	if (Ghost) {
		Ghost->Destroy(); Ghost = nullptr;
	}
}

void UPlayersTool::ChangeWeapon()
{
	DeleteItemFromHand();
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

void UPlayersTool::SetController(AHumanController* NewController, class UCameraComponent* NewCamera)
{
	DEBUG_CHECK_RETURN("UPlayersTool", NewController)

	PlayerController = NewController;
	PlayerController->PlayerEquipmentInventory->OnItemsChanged.AddUObject(this, &UPlayersTool::CheckItemsOnHand);
	PlayerCamera = NewCamera;

}

void UPlayersTool::CheckItemsOnHand()
{
switch (TecHandMode)
{
case Nothing:
	DeleteItemFromHand();

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
	if (WeaponOnHand && TecHandMode == EHandMode::HandlingWeapon)
		WeaponOnHand->StopAttak();
}

void UPlayersTool::Weapon_StartAttak()
{
	if (WeaponOnHand && TecHandMode == EHandMode::HandlingWeapon)
		WeaponOnHand->StartAttak();
}

void UPlayersTool::Weapon_SetAttaking(bool Value)
{
	if (WeaponOnHand && TecHandMode == EHandMode::HandlingWeapon)
		WeaponOnHand->SetAttaking(Value);
}

void UPlayersTool::Weapon_Attak()
{
	if (WeaponOnHand && TecHandMode == EHandMode::HandlingWeapon)
		WeaponOnHand->Attak();
}

