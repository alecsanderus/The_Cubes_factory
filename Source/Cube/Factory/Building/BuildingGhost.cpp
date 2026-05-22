#include "BuildingGhost.h"
#include "Cube/DebugMacros.h"

ABuildingGhost::ABuildingGhost()
{
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;
}

void ABuildingGhost::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ABuildingGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingGhost::SetColor(int NewColor)
{
	DEBUG_CHECK_RETURN("ABuildingGhost", BaseGhostMaterial);
	NewColor = (NewColor > 0) ? NewColor : 0;

	if (LastColor == -1)
	{
		
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseGhostMaterial, this);
		MainMesh->SetMaterial(0, DynamicMaterial);
		
	}
	if (NewColor == LastColor) return;
	DynamicMaterial->SetScalarParameterValue (ColorParamName, NewColor);

}

