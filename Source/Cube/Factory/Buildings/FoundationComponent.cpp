#include "FoundationComponent.h"

UFoundationComponent::UFoundationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UFoundationComponent::BeginPlay()
{
	Super::BeginPlay();
}



void UFoundationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

