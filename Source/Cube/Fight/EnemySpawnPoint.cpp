// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnPoint.h"
#include "Components/BillboardComponent.h"
// Sets default values
AEnemySpawnPoint::AEnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    rootcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));
    RootComponent = rootcomp;
//#if WITH_EDITORONLY_DATA
//    ObIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorIcon"));
//    ObIcon->SetupAttachment (RootComponent);
//
//    static ConstructorHelpers::FObjectFinder<UTexture2D> IconTextureObj(TEXT("/Game/Other/SpawnTextur.SpawnTextur"));
//    if (IconTextureObj.Succeeded())
//    {
//        ObIcon->Sprite = IconTextureObj.Object;
//    }
//    
//    SetActorScale3D(FVector(2, 2, 2));   
//    ObIcon->bIsScreenSizeScaled = true;
//    ObIcon->bIsEditorOnly = true; // чтобы компонент не попадал в игру
//    ObIcon->SetHiddenInGame(true);
//    ObIcon->SetRelativeLocation(FVector::ZeroVector);
//#endif

}

// Called when the game starts or when spawned
void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnPoint::SpawnEnemy(TSubclassOf<AActor> EnemyClass)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewEnemy = GetWorld()->SpawnActor<AActor>(
        EnemyClass, this->GetActorLocation(), this->GetActorRotation(), SpawnParams); 
    UE_LOG(LogTemp, Warning, TEXT("SPAWNPOINT LOC = %s"), *GetActorLocation().ToString());

}

