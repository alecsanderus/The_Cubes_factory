#include "ResoursePort.h"

UResoursePort::UResoursePort()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UResoursePort::BeginPlay()
{
	Super::BeginPlay();

	
}


void UResoursePort::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

