#include "ResourceExtractor.h"
#include "Cube/Factory/Building/BuildingComponent.h"
#include "Cube/WorldObjects/ResourceSourse.h"

AResourceExtractor::AResourceExtractor()
{
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;

	MyBuildingComponent = CreateDefaultSubobject <UBuildingComponent>(TEXT("BuildingComponent"));
	MyBuildingComponent->BindGetGhostPositionType([](const AActor* ac, const FTransform tr, const AActor* cd0) 
		{return AResourceExtractor::GetPosToSpawn(ac,tr, cd0); });
	MyBuildingComponent->ActorSnappedToActor.AddDynamic(this, &AResourceExtractor::SnapMeToSourse);
	MyBuildingComponent->CallType = ESnapCallType::OnlyOnSnapped;
	MyBuildingComponent->SnapToActor.Add(AResourceSourse::StaticClass());

}

void AResourceExtractor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AResourceExtractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AResourceExtractor::SnapMeToSourse(AActor* Sourse)
{
}

FTransform AResourceExtractor::GetPosToSpawn(const AActor* Object, const FTransform GhostTr, const AActor* CD0)
{
	if (!Object)
	{
		UE_LOG(LogTemp, Error, TEXT("FTransform AResourceExtractor::GetPosToSpawn HAS NO OBJECT, BRO ?!"));
		return FTransform();

	}
	return FTransform(GhostTr.Rotator(), Object->GetActorLocation() + FVector(0, 0, 100), FVector (1,1,1));
}

