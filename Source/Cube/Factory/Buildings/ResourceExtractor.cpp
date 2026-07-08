#include "ResourceExtractor.h"
#include "Cube/Factory/Building/BuildingComponent.h"
#include "Cube/WorldObjects/ResourceSourse.h"
#include "Cube/Factory/InteractComponent.h"
#include "Cube/Factory/FactorySubsystem.h"
#include "Cube/Factory/Items/ItemInfo.h"
#include "Cube/DebugMacros.h"
#include "GameFramework/Character.h"
#include "Cube/Player/HumanController.h"
#include "Cube/UI/Inventory/BuildingInventory.h"




AResourceExtractor::AResourceExtractor()
{
	PrimaryActorTick.bCanEverTick = false;

	MainMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;

	MyBuildingComponent = CreateDefaultSubobject <UBuildingComponent>(TEXT("BuildingComponent"));
	MyBuildingComponent->BindGetGhostPositionType([](const AActor* ac, const FTransform tr, const AActor* cd0) 
		{return AResourceExtractor::GetPosToSpawn(ac,tr, cd0); });

	MyBuildingComponent->ActorSnappedToActor.AddDynamic(this, &AResourceExtractor::SnapMeToSourse);

	MyBuildingComponent->CallType = ESnapCallType::OnlyOnSnapped;
	MyBuildingComponent->SnapToActor.Add(AResourceSourse::StaticClass());

	InteractComp = CreateDefaultSubobject <UInteractComponent>(TEXT("InteractComponent"));
	InteractComp->SetInteractFunction(std::bind(&AResourceExtractor::Interact, this, std::placeholders::_1));

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
	if (auto* surs = Cast <AResourceSourse>(Sourse))
	{
		DEBUG_CHECK_RETURN(AResourceExtractor, surs->ResourseData);
		auto* subsyst = GetWorld()->GetSubsystem <UFactorySubsystem>();

		FMachine mach{.MachineID = FGuid::NewGuid(), .MashineClass = GetClass(), .MachineTranform = GetTransform()};
		
		auto nam = surs->ResourseData->GetPrimaryAssetId();
		mach.Outputs = { {nam,0}};
		
		FName ReciepeText (nam.PrimaryAssetName.ToString() + TEXT("OnExtractor"));

		auto* reciepe = subsyst->RecipesNamesMap.Find(ReciepeText);
		if (reciepe)
			mach.RecipeID = *reciepe;
		else
		{
			FRecipe Reiep{.RecipeID = FGuid::NewGuid(), .Outputs = {{nam ,1}}, .TicksToCreate = 10, .Name = ReciepeText};
			subsyst->RegisterRecipe(Reiep);
			mach.RecipeID = Reiep.RecipeID;
		}
		MyBuildingComponent->MachineGuid = mach.MachineID;
		subsyst->RegisterMachine(mach);
	}
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

void AResourceExtractor::Interact(AActor* Caller)
{
	DEBUG_CHECK_RETURN(AResourceExtractor,MyBuildingComponent->MainInteractWidget);

	auto* subsyst = GetWorld()->GetSubsystem <UFactorySubsystem>();
	DEBUG_CHECK_RETURN(AResourceExtractor, subsyst);
	auto* Mach = subsyst->GetMachine(MyBuildingComponent->MachineGuid);
	DEBUG_CHECK_RETURN(AResourceExtractor, Mach);

	auto* PlCha = Cast <ACharacter> (Caller);
	if (!PlCha) return;
	auto* Contr = PlCha->GetController();
	if (!Contr) return;
	auto* HContr = Cast <AHumanController>(Contr);
	if (!HContr) return;

	auto* Widg = CreateWidget<UBuildingInventory>(GetWorld(), MyBuildingComponent->MainInteractWidget);
	HContr->OpenWindow(Widg, "AResourceExtractor");
	Widg->SetInventoryManager(MyBuildingComponent->MachineGuid);
	HContr->OpenAdditionalInventory();
}

