#include "DamageManager.h"
#include "GameFramework/Actor.h"


UDamageManager::UDamageManager()
{
    PrimaryComponentTick.bCanEverTick = false;

}

void UDamageManager::TakeDamage(float Damage, uint8 type)
{   
    TecHealth -= Damage;
    if (TecHealth <= 0)
    if (DieFunct) DieFunct();
}

void UDamageManager::BeginPlay()
{
    Super::BeginPlay();        
    TecHealth = MaxHealth;   
}

int UDamageManager::GetHealth()
{
    return  TecHealth;
}

int UDamageManager::GetMaxHealth()
{
    return  MaxHealth;
}

int UDamageManager::GetTeamID()
{
    return TeamID;
}

void UDamageManager::SetDieFunction(std::function<void()> NewDieFunct)
{
    DieFunct = NewDieFunct;
}

