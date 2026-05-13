#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoundationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUBE_API UFoundationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFoundationComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere)
	float SizeX = 200, SizeY = 200;
};
