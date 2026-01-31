#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ColdWeapon.generated.h"

class USplineComponent;

UCLASS()
class CUBE_API AColdWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	AColdWeapon();
protected:
	virtual void BeginPlay() override;
	TMap<TWeakObjectPtr<UDamageManager>, float> DamageCooldowns;
	float LastDeltaTime = 0;
public:

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void RegHit(AActor* Other);

	virtual void Tick(float DeltaTime) override;
	virtual void Attak() override;
	virtual void CreateAttak() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* MainComp;
	

	UPROPERTY(EditAnywhere)
	USplineComponent* AttackSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attak")
	float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attak")
	uint8 DamageType = 0;


	UPROPERTY(EditAnywhere, Category = "Attak")
	float AttackDuration = 0.4f;
		

	float CurrentAnTime = -1.f;

	FVector LastPos;
		
};
