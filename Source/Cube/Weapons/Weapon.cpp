// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Bullet.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject <USkeletalMeshComponent>(FName("MainMesh"));
    RootComponent = Mesh;

    ReloadTimer = -1;
    BulletsInMagasin = MagasinSize;
    QueueTimer = -1;
    BulletsInQueue = QueueSize;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();    
    if (!Mesh)   UE_LOG(LogTemp, Warning, TEXT("Weapon havent skeletal mesh    name: %s"), *GetName());
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (IsShooting)
    {
        Shoot();
    }
    CheckTimers();
}


void AWeapon::SetQueueShooting(bool Value)
{
    IsQueueShoot = Value;
}

void AWeapon::SetShooting(bool Value)
{
    IsShooting = Value;
}

void AWeapon::ReloadWeapon()
{
    ReloadTimer = GetWorld()->GetTimeSeconds() + ReloadTime;
    BulletsInMagasin = 0;
}

void AWeapon::SetAutoReload(bool Value)
{
    AutoReload = Value;
}

void AWeapon::SetMyOwner(AController* controller)
{
    MyOwnersController = controller;
}


void AWeapon::Shoot()
{
    double TecTime = GetWorld()->GetTimeSeconds();

    if (BulletsInMagasin <= 0 ||
        TecTime < ShootTimer ||
        (IsQueueShoot && BulletsInQueue <= 0)  )
        return;
    
     
        CreateShoot();


        if (IsQueueShoot)
        {
            --BulletsInQueue;
            if (BulletsInQueue <= 0)
                QueueTimer = TecTime + ShootSpeed;

            ShootTimer = TecTime + ShootQueueSpeed;
        }
        else
            ShootTimer = TecTime + StandatShootSpeed;

        --BulletsInMagasin;
        if (BulletsInMagasin <= 0 && AutoReload)
            ReloadWeapon();
}


void AWeapon::CreateShoot()
{
   /* if (Bullet)
    {
        FVector MuzzleLocation = Mesh->GetSocketLocation("Muzzle");
        FRotator MuzzleRotation = Mesh->GetSocketRotation("Muzzle");
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        ABullet* bull = GetWorld()->SpawnActor<ABullet>
            (Bullet, MuzzleLocation, MuzzleRotation, Params);
        bull->Shoot();
    }*/


    if (!Bullet) { UE_LOG(LogTemp, Warning, TEXT("Weapon hasnt bullet class   name: %s"), *GetName());   return; }
    if (!MyOwnersController) { UE_LOG(LogTemp, Warning, TEXT("Weapon hasnt controller class   name: %s"), *GetName());   return; }

    
    
    FTransform MuzzleTransform = Mesh->GetSocketTransform("Muzzle");
    FVector MuzzleLocation = MuzzleTransform.GetLocation();
    FVector MuzzleForward = MuzzleTransform.GetRotation().GetForwardVector();

   
    FVector AimDirection = MuzzleForward; 
    FVector TraceStart, TraceEnd;
    FHitResult HitResult;

    if (MyOwnersController && MyOwnersController->IsLocalController())
    {
        FVector CameraLocation;
        FRotator CameraRotation;
        MyOwnersController->GetPlayerViewPoint(CameraLocation, CameraRotation);

        TraceStart = CameraLocation;
        TraceEnd = TraceStart + CameraRotation.Vector() * 10000.f;

        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

      
        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
        {
            FVector HitLocation = HitResult.ImpactPoint;

            
            float DistanceToHit = FVector::Dist(MuzzleLocation, HitLocation);

            if (DistanceToHit < 150.f)
            {
             
                AimDirection = MuzzleForward;
            }
            else if (DistanceToHit < 500.f)
            {
              
                FVector CameraDir = (HitLocation - MuzzleLocation).GetSafeNormal();
                AimDirection = FMath::Lerp(MuzzleForward, CameraDir, 0.7f).GetSafeNormal();
            }
            else
            {
             
                AimDirection = (HitLocation - MuzzleLocation).GetSafeNormal();
            }
        }
        else
        {
            AimDirection = (TraceEnd - MuzzleLocation).GetSafeNormal();
        }
    }


    FVector SpawnLocation = MuzzleLocation;
    FRotator SpawnRotation = AimDirection.Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
   

    
    ABullet* Projectile = GetWorld()->SpawnActor<ABullet>(
        Bullet, SpawnLocation, SpawnRotation, SpawnParams);

    if (Projectile && Projectile->ProjectileMovement)
    {
        Projectile->ProjectileMovement->Velocity = AimDirection * Projectile->ProjectileMovement->InitialSpeed;
        Projectile->ProjectileMovement->bRotationFollowsVelocity = true;
        UE_LOG(LogTemp, Warning, TEXT("speeeds %f     %f   %f     %f   "), Projectile->ProjectileMovement->InitialSpeed, Projectile->ProjectileMovement->Velocity.X,
            Projectile->ProjectileMovement->Velocity.Y, Projectile->ProjectileMovement->Velocity.Z);
   
    }



}

void AWeapon::CheckTimers()
{
    double TecTime = GetWorld()->GetTimeSeconds();

    if (ReloadTimer != -1 && TecTime >= ReloadTimer)
    {
        ReloadTimer = -1;
        BulletsInMagasin = MagasinSize;
    }

    if (QueueTimer != -1 && TecTime >= QueueTimer)
    {
        QueueTimer = -1;
        BulletsInQueue = QueueSize;
    }
}

void AWeapon::StopShoot()
{
   
    IsShooting = 0;


}
void AWeapon::StartShoot()
{
    if (IsQueueShoot)
        IsShooting = 1;
    else
        Shoot();

}