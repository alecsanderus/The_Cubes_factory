#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HotWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CUBE_API AHotWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	AHotWeapon();
protected:

	virtual void BeginPlay() override;

	double ReloadTimer = -1.f;
	double QueueTimer = -1.f;

	unsigned int BulletsInMagasin;
	unsigned int BulletsInQueue;

	virtual void CreateAttak() override;
	virtual void CheckTimers();
public:

	virtual void Tick(float Deltaime) override;


	UFUNCTION(BlueprintCallable)
	void SetQueueShooting(bool Value);

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void SetAutoReload(bool Value);

	virtual void Attak() override;
	virtual void StartAttak() override;

	UPROPERTY(EditAnywhere, Category = "Attak")
	bool AutoReload = 1;

	UPROPERTY(EditAnywhere, Category = "Attak")
	bool IsQueueShoot = 0;

	UPROPERTY(EditAnywhere, Category = "Attak")
	unsigned int MagasinSize = 12;

	UPROPERTY(EditAnywhere, Category = "Attak", meta = (ClampMin = "0", UIMin = "0"))
	double ReloadTime = 2;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Attak")
	unsigned int QueueSize = 3;

	UPROPERTY(EditAnywhere, Category = "Attak", meta = (ClampMin = "0.0001", UIMin = "0.0001"))
	double ShootQueueSpeed = 0.1f;	

	UPROPERTY(EditAnywhere, Category = "Attak", meta = (ClampMin = "0", UIMin = "0"))
	double ShootForse = 10000.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf <ABullet> Bullet;
};
