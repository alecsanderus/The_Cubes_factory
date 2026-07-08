

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ResoursePort.generated.h"

UENUM(BlueprintType)
enum class EResoursePortType : uint8
{
	Solid,
	Fluid,
	Energy,
	Something
};

UENUM(BlueprintType)
enum class EResoursePortInOut : uint8
{
	In,
	Out,
	Nothing
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUBE_API UResoursePort : public USceneComponent
{
	GENERATED_BODY()

public:	
	UResoursePort();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	EResoursePortType MyType;

	UPROPERTY(EditAnywhere)
	EResoursePortInOut MyInOut;

};
