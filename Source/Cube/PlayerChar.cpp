// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Weapon.h"
#include "DamageManager.h"
#include "Bullet.h"
#include "DebugMacros.h"
#include "Cube/Factory/InteractComponent.h"



// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    
    // ќтключаем автоматический поворот тела персонажа за камерой
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = false;

    // ѕерсонаж не поворачиваетс€ сам Ч камера управл€ет всем
    GetCharacterMovement()->bOrientRotationToMovement = false;

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->bUsePawnControlRotation = true;
    CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f));

    
    MainMesh = GetMesh();
     
  
    MyDamageManager = CreateDefaultSubobject<UDamageManager>(TEXT("DamageManager"));    
              
  
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
    Super::BeginPlay();

    DEBUG_CHECK("PlayerChar", "HandligWeaponType", HandligWeaponType)
    {
        FVector WeaponLocation = MainMesh->GetSocketLocation("WeaponHand");
        FRotator WeaponRotation = MainMesh->GetSocketRotation("WeaponHand");

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        HandligWeapon = GetWorld()->SpawnActor<AWeapon>(HandligWeaponType, WeaponLocation, WeaponRotation, Params);
        HandligWeapon->AttachToComponent(CameraComponent, FAttachmentTransformRules::KeepWorldTransform, "WeaponHand");
        HandligWeapon->SetMyOwner(GetController());
    }           
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
    DEBUG_CHECK("PlayerChar", "HandligWeapon", HandligWeapon)
        HandligWeapon->StopAttak();
   
}

void APlayerChar::StartShoot()
{
    DEBUG_CHECK("PlayerChar", "HandligWeapon", HandligWeapon)
        HandligWeapon->StartAttak();

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


