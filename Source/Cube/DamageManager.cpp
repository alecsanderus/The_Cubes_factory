#include "DamageManager.h"
#include "GameFramework/Actor.h"


UDamageManager::UDamageManager()
{
    PrimaryComponentTick.bCanEverTick = false;


    // Создаём HitBox как дочерний компонент актора-владельца
   // HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageHitBox"));
    //HitBox->SetupAttachment(this);
   // if (HitBox)
   // {
        // Убедимся, что компонент корректно инициализирован, и только тогда добавляем привязку
       // HitBox->OnComponentBeginOverlap.AddDynamic(this, &UDamageManager::OnHitBoxOverlap);
      

        // Настройки коллизии
     //   HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
     //   HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
     //   HitBox->SetGenerateOverlapEvents(true);
   // }
        
    
}

void UDamageManager::TakeDamage(float Damage)
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

