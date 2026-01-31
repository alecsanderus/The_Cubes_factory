// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerChar.generated.h"

class AWeapon;
class UDamageManager;


UCLASS()
class CUBE_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerChar();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;


	


	UFUNCTION(BlueprintCallable, Category = "Input")
	void Move(const FInputActionValue& Value);

	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void StopShoot();

	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StartShoot();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void TryInteract(float TraceDistance = 5000.f);

	UFUNCTION(BlueprintCallable)
	int GetHealth();

	UFUNCTION(BlueprintCallable)
	int GetMaxHealth();



	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere, Category = "Input")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float SprintSpeed = 1200.f;
	
		
	

	UPROPERTY(EditAnywhere, Category = "Fight")
	TSubclassOf <AWeapon> HandligWeaponType;
	AWeapon* HandligWeapon;
	
	
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Fight")
	UDamageManager* MyDamageManager;

};
