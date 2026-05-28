#include "FoundationActor.h"
#include "Cube/Factory/Building/BuildingComponent.h"
#include "Engine/OverlapResult.h"
#include "Cube/DebugMacros.h"

AFoundationActor::AFoundationActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MainMesch = CreateDefaultSubobject <UStaticMeshComponent> (TEXT("MainMesh"));
	RootComponent = MainMesch;

	BuildingComp = CreateDefaultSubobject <UBuildingComponent>(TEXT("BuildingComp"));

	BuildingComp->BindGetGhostPositionType([](const AActor* ac, const FTransform tr, const AActor* cd0)
		{return AFoundationActor::GetPosToSpawn(ac, tr, cd0); });
	BuildingComp->ActorSnappedToActor.AddDynamic(this, &AFoundationActor::SnapMeToMe);
	
}

void AFoundationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFoundationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFoundationActor::SnapMeToMe(AActor* Sourse)
{}

FTransform AFoundationActor::GetPosToSpawn(const AActor* Object, const FTransform GhostTr, const AActor* CD0)
{
	if (!Object)
	{
		UE_LOG(LogTemp, Error, TEXT("FTransform AFoundationActor::GetPosToSpawn HAS NO OBJECT, BRO ?!"));
		return FTransform();

	}
	if (!CD0)
	{
		UE_LOG(LogTemp, Error, TEXT("FTransform AFoundationActor::GetPosToSpawn HAS NO CD0, BRO ?!"));
		return FTransform();

	}
	auto* CD0AA= Cast <AFoundationActor>(CD0);

	if (!CD0AA)
	{
		UE_LOG(LogTemp, Error, TEXT("FTransform AFoundationActor::GetPosToSpawn HAS NO CD0 with AFoundationActor, BRO ?!"));
		return FTransform();

	}

	FTransform trans = GhostTr;
	if (Cast <AFoundationActor>(Object))
		trans = CheckFoundationTransform(Object->GetTransform(), trans, CD0AA->FoundationSizeXY, CD0AA->FoundationSizeZ);
	else
	{

		const auto& act = GetNearestFoundation(GhostTr.GetLocation(), GhostTr.GetRotation(),
			Object->GetWorld(), nullptr, CD0AA->FoundationSizeXY, CD0AA->FoundationSizeZ);
		if (act)
		{
			trans = SnapTransformToGrid(act->GetTransform(), GhostTr);

			trans = CheckFoundationTransform(act->GetTransform(), trans, CD0AA->FoundationSizeXY, CD0AA->FoundationSizeZ);
		}
	}

	return trans;
}



FTransform AFoundationActor::SnapTransformToGrid(
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


FTransform AFoundationActor::CheckFoundationTransform(
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



AActor* AFoundationActor::GetNearestFoundation(
	FVector CheckLocation,
	FQuat CheckRotation,
	UWorld* WorldToCheck,
	AActor* IgnoreActor,
	double FoundationSizeXY,
	double FoundationSizeZ)
{

	if (!WorldToCheck) return nullptr;

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Cub = FCollisionShape::MakeBox(FVector3f(FoundationSizeXY / 2, FoundationSizeXY / 2, FoundationSizeZ / 2));

	FCollisionObjectQueryParams ObjectParams(FCollisionObjectQueryParams::InitType::AllObjects);

	bool bHit = WorldToCheck->OverlapMultiByChannel(
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
				if (Cast <AFoundationActor> (HitActor))
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

