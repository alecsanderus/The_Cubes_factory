

#include "CollectibleItem.h"
#include "InteractComponent.h"
#include "Cube/PlayerChar.h"
#include "Cube/HumanController.h"
#include "Cube/Factory/InventoryManager.h"

ACollectibleItem::ACollectibleItem()
{
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->SetInteractFunction(std::bind(&ACollectibleItem::Interact, this, std::placeholders::_1));
}

void ACollectibleItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACollectibleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectibleItem::Interact(UObject* Caller)
{
	APlayerChar* player = Cast <APlayerChar>(Caller);
	if (!player) return;
	AController* contr = player->GetController();
	if (!contr) return;
	AHumanController* Hcontr = Cast <AHumanController>(contr);
	if (!Hcontr) return;
	if (!Hcontr->PlayerInventory) return;
	Hcontr->PlayerInventory->AddItem(FInventoryItem{ ThisItem, amount }, -1);

	Destroy();
	
}


void ACollectibleItem::SetItem(UItemInfo* Item, int amoun)
{
	ThisItem = Item;
	amount = amoun;
	if (Item->Mesh) MainMesh->SetStaticMesh(Item->Mesh);
}
