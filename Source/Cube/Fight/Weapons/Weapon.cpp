#include "Weapon.h"
#include "Cube/DebugMacros.h"

// Sets default values
AWeapon::AWeapon()
{ 
	PrimaryActorTick.bCanEverTick = true;    
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();    
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (AutoAttakInTeak && IsAttaking)
    {
        Attak();
    }   
}



void AWeapon::SetAttaking(bool Value)
{   
    if (Value == IsAttaking) return;
    if (Value)
        StartAttak();
    else
        StopAttak();
}


void AWeapon::SetMyOwner(AController* controller)
{
    MyOwnersController = controller;
    APawn* Pawn = controller->GetPawn();
    if (Pawn)
    {
        UDamageManager* DM = Pawn->FindComponentByClass<UDamageManager>();
        if (DM)
        {
            MyDamageManager = DM;
        }
    }
}


void AWeapon::Attak()
{
    DEBUG_NOT_OVERRIDE("Attak()");
}


void AWeapon::CreateAttak()
{
    DEBUG_NOT_OVERRIDE("CreateAttak()")
}

void AWeapon::StopAttak()
{
    if (0 == IsAttaking) return;
    IsAttaking = 0;


}
void AWeapon::StartAttak()
{  
    if (1 == IsAttaking) return;
    IsAttaking = 1;   
}