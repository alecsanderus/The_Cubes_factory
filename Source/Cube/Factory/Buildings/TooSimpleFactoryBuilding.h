#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TooSimpleFactoryBuilding.generated.h"

UCLASS()
class CUBE_API ATooSimpleFactoryBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ATooSimpleFactoryBuilding();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
