#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceSourse.generated.h"

UCLASS()
class CUBE_API AResourceSourse : public AActor
{
	GENERATED_BODY()
	
public:	
	AResourceSourse();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY (EditAnywhere)
	class UItemInfo* ResourseData;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MainMesh;
};
