// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube/Weapons/Hands.h"
#include "Cube/DebugMacros.h"
AHands::AHands()
{    
    SkenComp = CreateDefaultSubobject <USceneComponent>(TEXT("Comp"));
    RootComponent = SkenComp;

    SkenCompF = CreateDefaultSubobject <USceneComponent>(TEXT("CompF"));
    SkenCompF->SetupAttachment(RootComponent);

    SkenCompS = CreateDefaultSubobject <USceneComponent>(TEXT("CompS"));
    SkenCompS->SetupAttachment(RootComponent);

    AutoAttakInTeak = 0;
}
void AHands::BeginPlay()
{
    Super::BeginPlay();   
    DEBUG_CHECK("Hands", "HandligWeaponType", HandsClass)
    {

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        FirstW = GetWorld()->SpawnActor<AWeapon>(HandsClass, SkenCompF->GetComponentLocation(), SkenCompF->GetComponentRotation(), Params);
        FirstW->AttachToComponent(SkenCompF, FAttachmentTransformRules::KeepWorldTransform);       

        SecondW = GetWorld()->SpawnActor<AWeapon>(HandsClass, SkenCompS->GetComponentLocation(), SkenCompS->GetComponentRotation(), Params);
        SecondW->AttachToComponent(SkenCompS, FAttachmentTransformRules::KeepWorldTransform);    

        AttakSpeed = FirstW->AttakSpeed/2;
    }
}

void AHands::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (IsAttaking && (AttakTimer <= CurrentTime))
    {
        Attak();
        AttakTimer = CurrentTime + AttakSpeed;      
    }
}


void AHands::SetMyOwner(AController* controller)
{
    Super::SetMyOwner(controller);
    FirstW->SetMyOwner(MyOwnersController);
    SecondW->SetMyOwner(MyOwnersController);
}

void AHands::Attak()
{
    if (Usinhron)
    {
        if (TecUsinhron)
            FirstW->Attak();
        else
            SecondW->Attak();
        TecUsinhron = !TecUsinhron;
    }
    else
    {       
        FirstW->Attak();       
        SecondW->Attak();
    }
}
