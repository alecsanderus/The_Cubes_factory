// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"


class UProjectileMovementComponent;

UCLASS()
class CUBE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Shoot();
	
	void SetDamage(int Value);
	void SetLifeTime(double Value);
	void SetForse(double Value);


	virtual void NotifyHit(
		class UPrimitiveComponent* MyComp, 
		AActor* Other,                     
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,                     
		FVector HitLocation,                 
		FVector HitNormal,                   
		FVector NormalImpulse,              
		const FHitResult& Hit            
	) override;



	UFUNCTION()
	void OnProjStop(const FHitResult& ImpactResult);

	UPROPERTY(EditAnywhere)
	UMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	double LifeTime = 30.f;


	UPROPERTY(EditAnywhere)
	double Forse = 1000.f;

	UPROPERTY(EditAnywhere)
	int Damage = 10;

	UPROPERTY(EditAnywhere)
	int MinSpeed = 100;

	UPROPERTY(EditAnywhere)
	bool IsHit = false;

	bool CanStop = 0;

	unsigned int TicsToStop = 0;

};
