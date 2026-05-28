#include "ResourceExtractor.h"
#include "Cube/Factory/Building/BuildingComponent.h"

AResourceExtractor::AResourceExtractor()
{
	PrimaryActorTick.bCanEverTick = true;
	MyBuildingComponent = CreateDefaultSubobject <UBuildingComponent>(TEXT("BuildingComponent"));
	MyBuildingComponent->BindGetGhostPositionType([](const AActor* ac, const FTransform tr, const AActor* cd0) 
		{return AResourceExtractor::GetPosToSpawn(ac,tr, cd0); });
	MyBuildingComponent->ActorSnappedToActor.AddDynamic(this, &AResourceExtractor::SnapMeToSourse);

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
	return FTransform(GhostTr.Rotator(), Object->GetActorLocation(), FVector (1,1,1));
}

