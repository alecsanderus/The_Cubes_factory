#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceExtractor.generated.h"

UCLASS()
class CUBE_API AResourceExtractor : public AActor
{
	GENERATED_BODY()
	
public:	
	AResourceExtractor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	class UBuildingComponent* MyBuildingComponent;

	UFUNCTION(BlueprintCallable)
	void SnapMeToSourse(AActor* Sourse);


	static FTransform GetPosToSpawn(const AActor* Object, const FTransform GhostTr, const AActor* CD0);

};
