

#include "CollectibleItem.h"
#include "Cube/Factory/InteractComponent.h"
#include "Cube/Player/PlayerChar.h"
#include "Cube/Player/HumanController.h"
#include "Cube/Factory/Items/InventoryManager.h"

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

void ACollectibleItem::Interact(AActor* Caller)
{
	APlayerChar* player = Cast <APlayerChar>(Caller);
	if (!player) return;
	AController* contr = player->GetController();
	if (!contr) return;
	AHumanController* Hcontr = Cast <AHumanController>(contr);
	if (!Hcontr) return;
	if (!Hcontr->PlayerInventory) return;
	int ye = Hcontr->PlayerInventory->AddItem(FInventoryItem{ ThisItem, amount });
	amount = ye;
	if (amount <= 0) Destroy();
	
}


void ACollectibleItem::SetItem(UItemInfo* Item, int amoun)
{
	ThisItem = Item;
	amount = amoun;
	if (Item->Mesh) MainMesh->SetStaticMesh(Item->Mesh);
}
