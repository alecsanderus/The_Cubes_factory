// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlayersTool.generated.h"

class AHumanController;
class AWeapon;

UENUM()
enum EHandMode
{
	Nothing,
	HandlingWeapon,
	Building,
	Destroying	
};


UCLASS()
class CUBE_API UPlayersTool : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	UPlayersTool();

protected:
	virtual void BeginPlay() override;
	AHumanController* PlayerController;
	AWeapon* WeaponOnHand;

public:	
	UPROPERTY()
	USceneComponent* MainComp;

	UFUNCTION ()
	void SetHandMode(EHandMode NewMode);

	UFUNCTION()
	void DoNothing();

	UFUNCTION()
	void ChangeWeapon();

	UFUNCTION()
	void SetController(AHumanController* NewController);

	UFUNCTION()
	void CheckItems();

	AActor* ObjectOnHand;

	EHandMode TecHandMode = EHandMode::HandlingWeapon;











	UFUNCTION(BlueprintCallable)
	void Weapon_StopAttak();

	UFUNCTION(BlueprintCallable)
	void Weapon_StartAttak();

	UFUNCTION(BlueprintCallable)
	void Weapon_SetAttaking(bool Value);

	UFUNCTION(BlueprintCallable)
	void Weapon_Attak();
};
