#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorSnappedToActor, AActor*, TotActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUBE_API UBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY (EditAnywhere)
	bool Destroyable = 1;

	UPROPERTY(EditAnywhere)
	TArray <TSubclassOf<AActor>> SnapToActor;

	UPROPERTY(EditAnywhere)
	TArray <TSubclassOf<AActor>> SnapToComponent;


	using FGetGhostPositionType = TFunction<FTransform(const AActor*, const FTransform, const AActor*)>;
	FGetGhostPositionType GetGhostPositionFF;

	void BindGetGhostPositionType(FGetGhostPositionType Funct) { GetGhostPositionFF = Funct; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnActorSnappedToActor ActorSnappedToActor;
};
