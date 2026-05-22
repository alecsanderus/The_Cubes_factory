#include "HotWeapon.h"
#include "Cube/DebugMacros.h"
#include "Bullet.h"

AHotWeapon::AHotWeapon()
{
    Mesh = CreateDefaultSubobject <USkeletalMeshComponent>(FName("MainMesh"));
    RootComponent = Mesh;

    ReloadTimer = -1;
    BulletsInMagasin = MagasinSize;
    QueueTimer = -1;
    BulletsInQueue = QueueSize;
}

void AHotWeapon::BeginPlay()
{
    Super::BeginPlay();
    DEBUG_CHECK_RETURN("Skeletal mesh", Mesh);
}


void AHotWeapon::Tick(float Deltaime)
{
    Super::Tick(Deltaime);
    CheckTimers();
}



void AHotWeapon::SetQueueShooting(bool Value)
{
    IsQueueShoot = Value;
}


void AHotWeapon::ReloadWeapon()
{
    ReloadTimer = GetWorld()->GetTimeSeconds() + ReloadTime;
    BulletsInMagasin = 0;
}

void AHotWeapon::SetAutoReload(bool Value)
{
    AutoReload = Value;
}

void AHotWeapon::Attak()
{
    double TecTime = GetWorld()->GetTimeSeconds();

    if (BulletsInMagasin <= 0 ||
        TecTime < AttakTimer ||
        (IsQueueShoot && BulletsInQueue <= 0))
        return;


    CreateAttak();


    if (IsQueueShoot)
    {
        --BulletsInQueue;
        if (BulletsInQueue <= 0)
            QueueTimer = TecTime + AttakSpeed;

        AttakTimer = TecTime + ShootQueueSpeed;
    }
    else
        AttakTimer = TecTime + AttakSpeed;

    --BulletsInMagasin;
    if (BulletsInMagasin <= 0 && AutoReload)
        ReloadWeapon();
}


void AHotWeapon::CreateAttak()
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

    
   
    DEBUG_CHECK_RETURN("Hot Weapon", (Bullet && MyOwnersController));

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

void AHotWeapon::CheckTimers()
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

void AHotWeapon::StartAttak()
{
    if (IsQueueShoot)
        IsAttaking = 1;
    else
        Attak();

}