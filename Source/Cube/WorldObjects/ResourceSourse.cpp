#include "ResourceSourse.h"

AResourceSourse::AResourceSourse()
{
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;	
}

void AResourceSourse::BeginPlay()
{
	Super::BeginPlay();
	
}

void AResourceSourse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

