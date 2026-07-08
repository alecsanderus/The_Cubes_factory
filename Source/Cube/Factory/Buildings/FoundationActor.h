

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoundationActor.generated.h"

UCLASS()
class CUBE_API AFoundationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoundationActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY (EditAnywhere)
	UStaticMeshComponent* MainMesch;

	UPROPERTY(EditAnywhere)
	class UBuildingComponent* BuildingComp;

	UPROPERTY(EditAnywhere)
	float FoundationSizeXY = 200;
	UPROPERTY(EditAnywhere)
	float FoundationSizeZ = 20;

	UFUNCTION(BlueprintCallable)
	void SnapMeToMe(AActor* Sourse);


	static FTransform GetPosToSpawn(const AActor* Object, const FTransform GhostTr, const AActor* CD0);


	static FTransform SnapTransformToGrid(
		const FTransform& GridCenter,
		const FTransform& ObjectTransform,
		double GridStepCm = 50,
		double GridStepDeg = 45);


	static FTransform CheckFoundationTransform(
		const FTransform& GridCenter,
		const FTransform& ObjectTransform,
		double FoundationSizeXY,
		double FoundationSizeZ);



	static AActor* GetNearestFoundation(
		FVector CheckLocation,
		FQuat CheckRotation,
		UWorld* WorldToCheck,
		AActor* IgnoreActor,
		double FoundationSizeXY,
		double FoundationSizeZ);

	
};
