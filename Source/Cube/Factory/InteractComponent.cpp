

#include "InteractComponent.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteractComponent::Interact(UObject* Caller)
{
	if (InteractFunct) InteractFunct(Caller);
}

void UInteractComponent::SetInteractFunction(std::function<void(UObject* Caller)> NewInteractFunct)
{
	InteractFunct = NewInteractFunct;
}

