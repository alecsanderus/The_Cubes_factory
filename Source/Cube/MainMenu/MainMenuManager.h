
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuManager.generated.h"

class ADirectionalLight;
class USpringArmComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UCameraComponent;
class UStaticMeshComponent;

UCLASS()
class CUBE_API AMainMenuManager : public APawn
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AMainMenuManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void PressButton(FString ButtonName);

	UPROPERTY (EditDefaultsOnly)
	UStaticMeshComponent* Cube;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* CubeMesh;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* Arm;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* Material;

	UPROPERTY(EditInstanceOnly)
	UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditInstanceOnly)
	ADirectionalLight* DirLight;

	UPROPERTY(EditInstanceOnly)
	float Time = 0;

	UPROPERTY(EditDefaultsOnly)
	FString MainLevelName;

	UPROPERTY(EditInstanceOnly)
	UUserWidget* MainWidget;

	UPROPERTY(EditInstanceOnly)
	UUserWidget* Settings;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf <UUserWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf <UUserWidget> SettingsWidgetClass;
};
