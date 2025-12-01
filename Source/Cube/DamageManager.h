#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Components/BoxComponent.h"
#include <functional>
#include "DamageManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_API UDamageManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UDamageManager();
    void TakeDamage (float Damage);

    UFUNCTION(BlueprintCallable)
    int GetHealth();

    UFUNCTION(BlueprintCallable)
    int GetMaxHealth();

    UFUNCTION(BlueprintCallable)
    int GetTeamID();

    UPROPERTY(EditAnywhere)
    int TeamID = 0;

    UPROPERTY(EditAnywhere)
    float MaxHealth = 100;

    UPROPERTY(EditAnywhere)
    float TecHealth;
         
    std::function<void()> DieFunct;

protected:
    virtual void BeginPlay() override;
  
   

};
