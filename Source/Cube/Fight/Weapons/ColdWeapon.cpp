// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdWeapon.h"
#include "Components/SplineComponent.h"
#include "Cube/Fight/DamageManager.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"

AColdWeapon::AColdWeapon()
{
    MainComp = CreateDefaultSubobject <USceneComponent>(TEXT("DefoltComp"));
    RootComponent = MainComp;
   


    AttackSpline = CreateDefaultSubobject<USplineComponent>(TEXT("AttackSpline"));
    AttackSpline->SetupAttachment(RootComponent);

    MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    MainMesh->SetupAttachment(RootComponent);
    MainMesh->SetSimulatePhysics(false);
    MainMesh->SetEnableGravity(false);
    MainMesh->SetNotifyRigidBodyCollision(true);     
    MainMesh->SetGenerateOverlapEvents(true);   
    MainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 

    AutoAttakInTeak = 0;
}

void AColdWeapon::BeginPlay()
{
    Super::BeginPlay();
    MainMesh->OnComponentHit.AddDynamic(this, &AColdWeapon::OnMeshHit);
    MainMesh->OnComponentBeginOverlap.AddDynamic(this, &AColdWeapon::OnMeshBeginOverlap);
}

void AColdWeapon::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    RegHit(OtherActor);
    UE_LOG(LogTemp, Warning, TEXT("HIT!!!!!!!!!!!!!!!!!!!!!!"));
}

void AColdWeapon::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    RegHit(OtherActor);
    UE_LOG(LogTemp, Warning, TEXT("Overlap&&&&&&&&&&&&&&&&&&&&&&"));
}


void AColdWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);   
    LastDeltaTime = DeltaTime;
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (IsAttaking && (CurrentAnTime == -1.f) && (CurrentTime >= AttakTimer))
    {
        AttakTimer = CurrentTime + AttakSpeed;
        CurrentAnTime = 0.f;
    }
    if ((CurrentAnTime != -1.f))
    {       
        CurrentAnTime += DeltaTime;
        CreateAttak();
    }
    else
    {      
        MainMesh->SetWorldLocation(AttackSpline->GetLocationAtTime(0.f, ESplineCoordinateSpace::World));
        MainMesh->SetWorldRotation(this->GetActorRotation());
    }
        
}

void AColdWeapon::Attak()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if ((CurrentAnTime == -1.f) && (CurrentTime >= AttakTimer))
    {
        AttakTimer = CurrentTime + AttakSpeed;
        CurrentAnTime = 0.f;       
    }
   
}

void AColdWeapon::CreateAttak()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float Alpha = CurrentAnTime / AttackDuration;
    FVector NewPos;
    if (Alpha >= 2.f)
    {
        CurrentAnTime = -1.f;
        NewPos = AttackSpline->GetLocationAtTime(0.f, ESplineCoordinateSpace::World);
        MainMesh->SetSimulatePhysics(0);
        MainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MainMesh->SetWorldLocation(NewPos);


        for (auto It = DamageCooldowns.CreateIterator(); It; ++It)
        {

            if (!It.Key().IsValid())
            {
                It.RemoveCurrent();
                continue;
            }


            if (It.Value() < CurrentTime)
            {
                It.RemoveCurrent();
            }
        }
       

    }
    else 
    {       
        NewPos = AttackSpline->GetLocationAtTime((Alpha >= 1.f) ? (2 - Alpha) : Alpha, ESplineCoordinateSpace::World);
        if (!MainMesh->IsSimulatingPhysics())
        {
            MainMesh->SetSimulatePhysics(1);
            MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
    }
  

   // MainMesh->SetWorldLocation(NewPos, true);


    
    // ---- Compute Velocity ----
    FVector Delta = NewPos - MainMesh->GetComponentLocation ();
    FVector Velocity = Delta / LastDeltaTime;

    // ---- Apply velocity to physical body ----
    MainMesh->SetPhysicsLinearVelocity(Velocity, false);

    LastPos = NewPos;
        
}


void AColdWeapon::RegHit(AActor* Other)
{  
    float Alpha = CurrentAnTime / AttackDuration;
    if (Alpha > 1) return;
    UE_LOG(LogTemp, Warning, TEXT("Notif"));
    if (Other && Other != this)
    {
        UDamageManager* DamManager = Other->FindComponentByClass<UDamageManager>();
        if (DamManager && DamManager->GetTeamID() != MyDamageManager->GetTeamID() && DamManager->GetTeamID() != 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Damag"));
            float CurrentTime = GetWorld()->GetTimeSeconds();

            if (DamageCooldowns.Contains(DamManager))
            {
                
                float NextTime = DamageCooldowns[DamManager];
                if (CurrentTime < NextTime)
                {
                    return;
                }
            }
            UE_LOG(LogTemp, Warning, TEXT("Attak"));
            DamManager->TakeDamage(Damage, DamageType);

            DamageCooldowns.Add(DamManager, CurrentTime + (AttackDuration*0.3f));
        }

    }
}
