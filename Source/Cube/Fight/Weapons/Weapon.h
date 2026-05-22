// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube/Fight/DamageManager.h"
#include "Weapon.generated.h"

class ABullet;

UCLASS()
class CUBE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();

protected:

	virtual void BeginPlay() override;
	
	double AttakTimer = -1.f;	
	bool IsAttaking = 0;
	AController* MyOwnersController = nullptr;
	bool AutoAttakInTeak = 1;
	UDamageManager* MyDamageManager;
	virtual void CreateAttak();
public:	

	virtual void Tick(float DeltaTime) override;
	

	UFUNCTION(BlueprintCallable)
	virtual void StopAttak();	

	UFUNCTION(BlueprintCallable)
	virtual void StartAttak();
		
	UFUNCTION(BlueprintCallable)
	virtual void SetAttaking(bool Value);

	UFUNCTION(BlueprintCallable)
	virtual void Attak();
	
	UFUNCTION(BlueprintCallable)
	virtual void SetMyOwner(AController * controller);

	UPROPERTY(EditAnywhere, Category = "Attak", meta = (ClampMin = "0.0001", UIMin = "0.0001"))
	double AttakSpeed = 0.5f;
		
};
