// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AWeapon;
class AAIController;
class UDamageManager;

UCLASS()
class CUBE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AAIController* MyAIControoler;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float RotationSpeed = 10.0f;

	AActor* PlayerActor;
	bool bCanSeePlayer = false;
	FVector LastKnownLocation;
	FVector CurrentDestination;

	void MoveToNextPatrolPoint();	
	void RotateToPlayer(float DeltaTime);
	void Shoot(); // Пока пусто
	void LosePlayer();
	void FindPlayer();

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(EditAnywhere)
	TSubclassOf <AWeapon> HandligWeaponType;
	AWeapon* HandligWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* MainMesh;
	
};