// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cube/Weapons/Weapon.h"
#include "hands.generated.h"

/**
 * 
 */
UCLASS()
class CUBE_API Ahands : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void Shoot() override;
	virtual void CreateShoot() override;
};
