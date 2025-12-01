// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABullet;

UCLASS()
class CUBE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool IsShooting = 0;
	UPROPERTY(EditAnywhere)
	bool AutoReload = 1;
	UPROPERTY(EditAnywhere)
	bool IsQueueShoot = 0;

	double ReloadTimer = -1.f;
	double QueueTimer = -1.f;
	double ShootTimer = -1.f;

	unsigned int BulletsInMagasin;
	unsigned int BulletsInQueue;

	virtual void CreateShoot();
	virtual void CheckTimers();
public:	
	// Called every frame
	virtual void Tick(float Deltaime) override;
	
	void StopShoot();	
	void StartShoot();

	void SetQueueShooting(bool Value);
	void SetShooting(bool Value);
	virtual void Shoot();
	void ReloadWeapon();
	void SetAutoReload(bool Value);
	void SetMyOwner(AController * controller);

	UPROPERTY(EditAnywhere)
	unsigned int MagasinSize = 12;

	UPROPERTY(EditAnywhere)
	double ReloadTime = 2;

	UPROPERTY(EditAnywhere)
	double ShootSpeed = 0.5f;

	UPROPERTY(EditAnywhere)
	unsigned int QueueSize = 3;

	UPROPERTY(EditAnywhere)
	double ShootQueueSpeed = 0.1f;

	UPROPERTY(EditAnywhere)
	double StandatShootSpeed = 0.4f;

	UPROPERTY(EditAnywhere)
	double ShootForse = 10000.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf <ABullet> Bullet;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly)
	AController* MyOwnersController = nullptr;
};
