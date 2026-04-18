#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingGhost.generated.h"

UCLASS()
class CUBE_API ABuildingGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildingGhost();

protected:
	virtual void BeginPlay() override;
	int LastColor = -1;
public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY (EditAnywhere)
	UStaticMeshComponent* MainMesh;



	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* BaseGhostMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	static constexpr TCHAR ColorParamName[] = TEXT("Color");

	UFUNCTION()
	void SetColor(int NewColor);
};
