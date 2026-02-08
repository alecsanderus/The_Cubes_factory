// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayersTool.generated.h"

class AHumanController;


UENUM()
enum EHandMode
{
	Nothing,
	HandlingWeapon,
	Building,
	Destroying	
};


UCLASS()
class CUBE_API APlayersTool : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayersTool();

protected:
	virtual void BeginPlay() override;
	AHumanController* PlayerController;

public:	
	UPROPERTY()
	USceneComponent* MainComp;

	virtual void Tick(float DeltaTime) override;

	void SetHandMode(EHandMode NewMode);

	void DoNothing();
	void ChangeWeapon();
	void SetController(AHumanController* NewController);

	AActor* ObjectOnHand;

	EHandMode TecHandMode = EHandMode::HandlingWeapon;
};
