// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Hands.generated.h"

/**
 * 
 */
UCLASS()
class CUBE_API AHands : public AWeapon
{
	GENERATED_BODY()
public:
	AHands();
protected:
	virtual void BeginPlay() override;
	bool TecUsinhron = 1;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetMyOwner(AController* controller) override;

	virtual void Attak() override;

	UPROPERTY(EditAnywhere)
	bool Usinhron = 1;

	UPROPERTY(EditAnywhere)
	TSubclassOf <AWeapon> HandsClass;

	UPROPERTY (EditInstanceOnly)
	AWeapon* FirstW;

	UPROPERTY(EditInstanceOnly)
	AWeapon* SecondW;

	UPROPERTY(EditAnywhere)
	USceneComponent* SkenComp;

	UPROPERTY(EditAnywhere)
	USceneComponent* SkenCompF;

	UPROPERTY(EditAnywhere)
	USceneComponent* SkenCompS;




};
