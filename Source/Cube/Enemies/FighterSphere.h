// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "list"
#include "FighterSphere.generated.h"

class UDamageManager;
class UGeometryCollectionComponent;
UCLASS()
class CUBE_API AFighterSphere : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFighterSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;    

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



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



    UFUNCTION(BlueprintCallable, Category = "Movement")
    void JumpToTarget(const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Move(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Look(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocation(const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveAlongPath(float DeltaTime);



    UFUNCTION(BlueprintCallable)
    void Die();




    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* BallMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* BallVisMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UDamageManager* MyDamageManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UGeometryCollectionComponent* DestoyeblComp;




    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0", UIMin = "0"))
    float RollForce = 50000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0", UIMin = "0"))
    float JumpDistance = 200.0f;    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0", UIMin = "0"))
    float JumpImpulse = 800.0f;



    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* GameplayMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;




    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attak")
    float Damage = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attak", meta = (ClampMin = "0.0001", UIMin = "0.0001"))
    float DamageCooldown = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attak")
    uint8 DamageType = 0;


protected:

    TArray<FVector> PathPoints;
    int32 CurrentPathIndex = 0;

    TMap<TWeakObjectPtr<UDamageManager>, float> DamageCooldowns;

    float FindPathTimer = 0;   

    APawn* PlayerPawn;

    bool IsDie = 0;
};
