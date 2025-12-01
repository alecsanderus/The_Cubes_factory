// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuManager.h"
#include "Engine/DirectionalLight.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/Material.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "UMG.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
AMainMenuManager::AMainMenuManager()
{
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cube"));
	RootComponent = Cube;
	Arm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	Arm->SetupAttachment(Cube);
	Camera = CreateDefaultSubobject <UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(Arm);
}


// Called when the game starts
void AMainMenuManager::BeginPlay()
{
	Super::BeginPlay();

	if (!(Cube && CubeMesh && Camera && Arm && Material))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cube hasnt data    name: %s"), *GetName());
		return;
	}
	Cube->SetStaticMesh(CubeMesh);	
	Cube->SetMaterial(0, Material);
	

	DynMaterial = Cube->CreateAndSetMaterialInstanceDynamic(0);
	

	

	for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
	{
		DirLight = *It;
		break; // Если нужно только одно солнце
	}

	Time = 200;

	if (IsValid(MainWidgetClass))
	{
		MainWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true; // Показываем курсор
		PC->SetInputMode(FInputModeUIOnly()); // Ввод только в UI
	}

}

void AMainMenuManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Arm->AddLocalRotation
	Arm->AddRelativeRotation(FRotator(0, DeltaTime, 0));	

	Time += DeltaTime * 0.5f;

	if (Time >= 360.f) Time = 0;
	if (DirLight)
	{
		DirLight->SetActorRotation(FRotator(Time, 40, 0));
		if (DynMaterial)
		{
			float inten;
			if (Time > 180.f)
				inten = 0.f;
			else if (Time < 20.f)
				inten = Time * 0.05f;
			else if (Time > 160.f)
				inten = (180.f - Time) * 0.05f;
			else
				inten = 1.f;

			DynMaterial->SetScalarParameterValue(FName("Light"), inten);
		}
	}
}

void AMainMenuManager::PressButton(FString ButtonName)
{
	UE_LOG(LogTemp, Warning, TEXT("ButtonPressed   name: %s"), *ButtonName);
	if (ButtonName == "StartGame")
	{
		UGameplayStatics::OpenLevel(this, FName("/Game/Maps/FightLevel1/Level1"));
		return;
	}
	if (ButtonName == "Exit")
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), Cast <APlayerController>( GetController()), EQuitPreference::Quit, true);
		return;
	}
}

