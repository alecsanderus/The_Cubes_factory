// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Cube/Fight/Weapons/Weapon.h"
#include "Cube/Fight/DamageManager.h"
#include "Cube/Fight/Weapons/Bullet.h"
#include "Cube/DebugMacros.h"
#include "Cube/Factory/InteractComponent.h"
#include "HumanController.h" 
#include "PlayersTool.h"



APlayerChar::APlayerChar()
{
	PrimaryActorTick.bCanEverTick = true;
    
    
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->bUsePawnControlRotation = true;
    CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f));

    
    MainMesh = GetMesh();

  
    
    PlayerTool = CreateDefaultSubobject <UPlayersTool>(TEXT("CubeTool"));

    PlayerTool->SetupAttachment(CameraComponent);
    
   
   

    MyDamageManager = CreateDefaultSubobject<UDamageManager>(TEXT("DamageManager"));    
              
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    }
}


void APlayerChar::BeginPlay()
{
    Super::BeginPlay();

    DEBUG_CHECK("APlayerChar", PlayerTool)
    {
        PlayerTool->SetWorldTransform(MainMesh->GetSocketTransform("WeaponHand"));
        if (auto* contr = Cast <AHumanController>(GetController()))
            PlayerTool->SetController(contr, CameraComponent);


        PlayerTool->SetHandMode(EHandMode::HandlingWeapon);






        PlayerTool->CheckItemsOnHand();
    }
    //DEBUG_CHECK("PlayerChar", HandligWeaponType)
    //{
    //    FVector WeaponLocation = MainMesh->GetSocketLocation("WeaponHand");
    //    FRotator WeaponRotation = MainMesh->GetSocketRotation("WeaponHand");

    //    FActorSpawnParameters Params;
    //    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //    HandligWeapon = GetWorld()->SpawnActor<AWeapon>(HandligWeaponType, WeaponLocation, WeaponRotation, Params);
    //    HandligWeapon->AttachToComponent(CameraComponent, FAttachmentTransformRules::KeepWorldTransform, "WeaponHand");
    //    HandligWeapon->SetMyOwner(GetController());
    //}      

}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APlayerChar::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        AddMovementInput(GetActorRightVector(), MovementVector.Y);
        AddMovementInput(GetActorForwardVector(), MovementVector.X);
    }
}

void APlayerChar::Look(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(MovementVector.X);
        AddControllerPitchInput(-MovementVector.Y);
    }
}

void APlayerChar::StopShoot()
{
    DEBUG_CHECK("APlayerChar", PlayerTool)
    PlayerTool->Weapon_StopAttak();
}

void APlayerChar::StartShoot()
{
    DEBUG_CHECK("APlayerChar", PlayerTool)
    PlayerTool->Weapon_StartAttak();
}

void APlayerChar::TryInteract(float TraceDistance)
{

    UWorld* World = GetWorld();
    if (!World) return;


    FVector Start = CameraComponent->GetComponentLocation();
    FVector Forward = CameraComponent->GetForwardVector();

     
    FVector End = Start + Forward * TraceDistance;

  
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);    

    FHitResult Hit;

    if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        AActor* actr =  Hit.GetActor();

        UInteractComponent* comp = actr->FindComponentByClass<UInteractComponent>();
        if (comp)
        {
            comp->Interact(this);
        }
    }

}

int APlayerChar::GetHealth()
{
    return 0;// MyDamageManager->TecHealth;
}

int APlayerChar::GetMaxHealth()
{
    return 0; // MyDamageManager->MaxHealth;
}

void APlayerChar::StartJumping()
{
    Jump();
}

void APlayerChar::StopJump()
{
    StopJumping();
}

void APlayerChar::StartCrouch()
{
    Crouch();
}

void APlayerChar::StopCrouch()
{
    UnCrouch();
}


