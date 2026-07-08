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
	Destroying,
	TurnBack
};

enum class ESnapCallType : uint8;

UCLASS()
class CUBE_API UPlayersTool : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	UPlayersTool();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditInstanceOnly)
	AHumanController* PlayerController;

	UPROPERTY(EditInstanceOnly)
	class UCameraComponent* PlayerCamera;

	UPROPERTY(EditInstanceOnly)
	AWeapon* WeaponOnHand;

	UPROPERTY(EditInstanceOnly)
	AActor* ObjectOnHand;

	UPROPERTY(EditInstanceOnly)
	AActor* LastSnappedActor;

	UPROPERTY(EditInstanceOnly)
	class ABuildingGhost* Ghost;

	EHandMode OldTecHandMode = EHandMode::HandlingWeapon;

	UPROPERTY(EditInstanceOnly)
	class UBuildingConfig* TecBuildingConfig;
		
	bool BuildingOK = 1;
	TFunction<FTransform(const AActor*, const FTransform, const AActor*)> BuildigTransformFunction;

	UPROPERTY(EditInstanceOnly)
	AActor* BuildingCD0;

	const TArray <TSubclassOf<AActor>>* SnapToActor;

	const TArray <TSubclassOf<UActorComponent>>* SnapToComponent;

	const ESnapCallType* CallType;

public:	
	UPROPERTY()
	USceneComponent* MainComp;


	UFUNCTION ()
	void SetHandMode(EHandMode NewMode, UObject* param = nullptr);

	UFUNCTION()
	void DeleteItemFromHand();

	UFUNCTION()
	void ChangeWeapon();

	UFUNCTION()
	void SetController(AHumanController* NewController, class UCameraComponent* NewCamera);

	void CheckItemsOnHand();
	void CheckItemsOnHand(int32 Index);


	UFUNCTION()
	void ConfirmBuilding();


	

	
	EHandMode TecHandMode = EHandMode::HandlingWeapon;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf <class ABuildingGhost> BuildingGhostClass;

	UPROPERTY(EditAnywhere)
	float BuildingMaxDistance = 5000.f;


	UFUNCTION(BlueprintCallable)
	void Weapon_StopAttak();

	UFUNCTION(BlueprintCallable)
	void Weapon_StartAttak();

	UFUNCTION(BlueprintCallable)
	void Weapon_SetAttaking(bool Value);

	UFUNCTION(BlueprintCallable)
	void Weapon_Attak();

	FTransform SnapTransformToGrid(
		const FTransform& GridCenter,
		const FTransform& ObjectTransform,
		double GridStepCm = 50,
		double GridStepDeg = 45);




	bool IsObjectDuplicate(
		TSubclassOf<AActor> ObjectClass,
		const FTransform& TargetTransform,
		float DistanceThreshold = 10.0f,
		float AngleThresholdDeg = 5.0f);
	
};
