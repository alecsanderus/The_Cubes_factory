#include "PlayersTool.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "Cube/Player/HumanController.h"
#include "Cube/Factory/Items/InventoryManager.h"
#include "Cube/Fight/Weapons/Weapon.h"
#include "Cube/Factory/Building/BuildingGhost.h"
#include "Cube/Factory/Building/BuildingConfig.h"
#include "Cube/DebugMacros.h"
#include "Camera/CameraComponent.h"
#include "Cube/Factory/Buildings/FoundationComponent.h"
#include "Engine/OverlapResult.h"


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
			bool ok = 1;
			if (auto com = Hit.GetComponent())
			{
				if (auto act = com->GetOwner())
				{
					if (auto GG = act->GetComponentByClass <UFoundationComponent>())
					{
						ok = 0;
						FTransform trans = SnapTransformToGrid(act->GetTransform(), FTransform(Ghost->GetActorRotation(), Hit.Location, { 1,1,1 }));
						if (IsBuildingFoundation)
						{
							trans = CheckFoundationTransform(act->GetTransform(), trans, BuildingFoundationSizeXY, BuildingFoundationSizeZ);
						}
						Ghost->SetActorTransform(trans);
					}

				}

			}
			if (ok)
			{
				if (IsBuildingFoundation)
				{
					const auto& act = GetNearestFoundation(Hit.Location, Ghost->GetActorRotation().Quaternion(),
						BuildingFoundationSizeXY, BuildingFoundationSizeZ, Ghost);
					if (act)
					{
						FTransform trans = SnapTransformToGrid(act->GetTransform(), FTransform(Ghost->GetActorRotation(), Hit.Location, { 1,1,1 }));
					
						trans = CheckFoundationTransform(act->GetTransform(), trans, BuildingFoundationSizeXY, BuildingFoundationSizeZ);
						
						Ghost->SetActorLocation(Hit.Location);
						Ghost->SetActorTransform(trans);
					}
					else
						Ghost->SetActorLocation(Hit.Location);
				}
				else
					Ghost->SetActorLocation(Hit.Location);
			}

			if (Ghost->IsHidden())
				Ghost->SetActorHiddenInGame(false);
		}
		else
		{
			if (!Ghost->IsHidden())
				Ghost->SetActorHiddenInGame(true);
		}
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
		TecBuildingConfig = conf;


		const AActor* ActorCDO = conf->Object->GetDefaultObject<AActor>();

		if (ActorCDO)
		{
			UFoundationComponent* FoundComp = nullptr;

			FoundComp = ActorCDO->FindComponentByClass<UFoundationComponent>();

			if (!FoundComp)
			{
				if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(ActorCDO->GetClass()))
				{
					if (USimpleConstructionScript* SCS = BPClass->SimpleConstructionScript)
					{
						for (USCS_Node* Node : SCS->GetAllNodes())
						{
							if (Node->ComponentTemplate && Node->ComponentTemplate->IsA<UFoundationComponent>())
							{
								FoundComp = Cast<UFoundationComponent>(Node->ComponentTemplate);
								break;
							}
						}
					}
				}
			}

			if (FoundComp)
			{
				IsBuildingFoundation = 1;
				BuildingFoundationSizeXY = FoundComp->SizeXY;
				BuildingFoundationSizeZ = FoundComp->SizeZ;
			}
			else
			{
				IsBuildingFoundation = 0;
			}
		}


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

void UPlayersTool::ConfirmBuilding()
{
	if (!(TecHandMode == Building && Ghost && TecBuildingConfig)) return;
	DEBUG_CHECK_RETURN("UPlayersTool", TecBuildingConfig->Object);
	FActorSpawnParameters param;;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	

	GetWorld()->SpawnActor <AActor>(TecBuildingConfig->Object, Ghost->GetActorLocation(),Ghost->GetActorRotation(), param);
}

void UPlayersTool::Weapon_StopAttak()
{
	switch (TecHandMode)
	{
	case Nothing:
		break;
	case HandlingWeapon:
	{
		if (WeaponOnHand)
			WeaponOnHand->StopAttak();
	}
		break;
	case Building:
		break;
	case Destroying:
		break;
	case TurnBack:
		break;
	default:
		break;
	}

	
}

void UPlayersTool::Weapon_StartAttak()
{
	switch (TecHandMode)
	{
	case Nothing:
		break;
	case HandlingWeapon:
	{
		if (WeaponOnHand)
			WeaponOnHand->StartAttak();
	}
	break;
	case Building:
	{
		ConfirmBuilding();
	}
		break;
	case Destroying:
		break;
	case TurnBack:
		break;
	default:
		break;
	}
}

void UPlayersTool::Weapon_SetAttaking(bool Value)
{
	if (WeaponOnHand && TecHandMode == EHandMode::HandlingWeapon)
		WeaponOnHand->SetAttaking(Value);
	else if (Value) Weapon_StartAttak();
	else Weapon_StopAttak();
}

void UPlayersTool::Weapon_Attak()
{
	if (WeaponOnHand && TecHandMode == EHandMode::HandlingWeapon)
		WeaponOnHand->Attak();
}



FTransform UPlayersTool::SnapTransformToGrid(
	const FTransform& GridCenter,
	const FTransform& ObjectTransform,
	double GridStepCm,
	double GridStepDeg)
{
	FTransform LocalTransform =
		ObjectTransform.GetRelativeTransform(GridCenter);


	FVector LocalLocation = LocalTransform.GetLocation();

	LocalLocation.X = FMath::GridSnap(LocalLocation.X, GridStepCm);
	LocalLocation.Y = FMath::GridSnap(LocalLocation.Y, GridStepCm);
	//LocalLocation.Z = FMath::GridSnap(LocalLocation.Z, GridStepCm);

	

	FRotator LocalRotation =
		LocalTransform.GetRotation().Rotator();

	LocalRotation.Normalize();

	LocalRotation.Pitch =
		FMath::GridSnap(LocalRotation.Pitch, GridStepDeg);

	LocalRotation.Yaw =
		FMath::GridSnap(LocalRotation.Yaw, GridStepDeg);

	LocalRotation.Roll =
		FMath::GridSnap(LocalRotation.Roll, GridStepDeg);

	LocalRotation.Normalize();


	FTransform SnappedLocalTransform(
		LocalRotation,
		LocalLocation,
		LocalTransform.GetScale3D());


	return SnappedLocalTransform * GridCenter;
}


FTransform UPlayersTool::CheckFoundationTransform(
	const FTransform& GridCenter,
	const FTransform& ObjectTransform,
	double FoundationSizeXY,
	double FoundationSizeZ)
{

	FVector LocalLocation = GridCenter.InverseTransformPosition(ObjectTransform.GetLocation());

	const double HalfSize = FoundationSizeXY * 0.5;
	const double EdgeThreshold = HalfSize * 0.4;

	double AbsX = FMath::Abs(LocalLocation.X);
	double AbsY = FMath::Abs(LocalLocation.Y);

	FVector SnapOffset = FVector::ZeroVector;

	if (AbsX > AbsY)
	{
		if (LocalLocation.X > EdgeThreshold)
		{
			SnapOffset = FVector(FoundationSizeXY, 0.f, 0.f);
		}
		else if (LocalLocation.X < -EdgeThreshold)
		{
			SnapOffset = FVector(-FoundationSizeXY, 0.f, 0.f);
		}

	}
	else
	{

		if (LocalLocation.Y > EdgeThreshold)
		{
			SnapOffset = FVector(0.f, FoundationSizeXY, 0.f);
		}
		else if (LocalLocation.Y < -EdgeThreshold)
		{
			SnapOffset = FVector(0.f, -FoundationSizeXY, 0.f);
		}
	}
	
	


	FVector WorldLocation = GridCenter.TransformPosition(SnapOffset);

	if (SnapOffset == FVector::ZeroVector)
	{
		if (ObjectTransform.GetLocation().Z > GridCenter.GetLocation().Z + (FoundationSizeZ / 2))
			WorldLocation.Z = ObjectTransform.GetLocation().Z;
		else 
			WorldLocation.Z = ObjectTransform.GetLocation().Z - FoundationSizeZ;
	}

	FQuat WorldRotation = GridCenter.GetRotation();

	return FTransform(WorldRotation, WorldLocation, FVector::OneVector);
}



AActor* UPlayersTool::GetNearestFoundation(
	FVector CheckLocation,
	FQuat CheckRotation,
	double FoundationSizeXY,
	double FoundationSizeZ,
	AActor* IgnoreActor)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Cub = FCollisionShape::MakeBox (FVector3f (FoundationSizeXY/2, FoundationSizeXY/2, FoundationSizeZ/2));

	FCollisionObjectQueryParams ObjectParams(FCollisionObjectQueryParams::InitType::AllObjects);

	bool bHit = World->OverlapMultiByChannel(
		OverlapResults,
		CheckLocation,
		CheckRotation,
		ECC_Visibility,
		Cub
	);
	

	AActor* ClosestActor = nullptr;
	double MinDistanceSq = FLT_MAX;

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();

			if (HitActor && HitActor != IgnoreActor)
			{				
				if (HitActor->FindComponentByClass<UFoundationComponent>())
				{
					double CurrentDistanceSq = FVector::DistSquared(CheckLocation, HitActor->GetActorLocation());

					if (CurrentDistanceSq < MinDistanceSq)
					{
						MinDistanceSq = CurrentDistanceSq;
						ClosestActor = HitActor;
					}
				}
			}
		}
	}

	return ClosestActor;
}