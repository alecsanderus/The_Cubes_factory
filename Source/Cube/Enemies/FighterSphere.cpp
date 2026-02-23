// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterSphere.h"
#include "Cube/DebugMacros.h"
#include "GameFramework/Controller.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "Cube/Fight/DamageManager.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "GeometryCollection/GeometryCollectionActor.h"


// Sets default values
AFighterSphere::AFighterSphere()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = BallMesh;
	BallMesh->SetSimulatePhysics(true);
    BallMesh->SetCanEverAffectNavigation(false);
	BallMesh->SetAngularDamping(0.1f);
	BallMesh->SetLinearDamping(0.1f);
	BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallMesh->SetVisibility(false);

    BallVisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallVisibleMesh"));
    BallVisMesh->SetupAttachment(RootComponent);
    BallVisMesh->SetSimulatePhysics(false);
    BallVisMesh->SetCanEverAffectNavigation(false);    
    BallVisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MyDamageManager = CreateDefaultSubobject<UDamageManager>(TEXT("DamageManager"));
    MyDamageManager->DieFunct = std::bind(&AFighterSphere::Die, this);

    DestoyeblComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestoyeblComponent"));
    DestoyeblComp->SetupAttachment(RootComponent);
    DestoyeblComp->SetSimulatePhysics(false);
    DestoyeblComp->SetVisibility(false);
    IsDie = 0;
}

// Called when the game starts or when spawned
void AFighterSphere::BeginPlay()
{
	Super::BeginPlay();
    DEBUG_CHECK("PlayerChar", "GameplayMappingContext", GameplayMappingContext)
    {
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
            {
                Subsystem->AddMappingContext(GameplayMappingContext, 0);
            }
        }
    }
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    MoveToLocation(PlayerPawn->GetActorLocation());    

}


void AFighterSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (IsDie) return;

    float CurrentTime = GetWorld()->GetTimeSeconds();

    MoveAlongPath(DeltaTime);

    if (FindPathTimer <= CurrentTime)
    {
        FindPathTimer = CurrentTime + 1.f;       
        MoveToLocation(PlayerPawn->GetActorLocation());




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




    /*FVector InputDir(
        MovementVector.X,
        MovementVector.Y,
        0.0f
    );

    if (!InputDir.IsNearlyZero())
    {
        InputDir.Normalize();
        BallMesh->AddTorqueInRadians(InputDir * RollForce * DeltaTime, NAME_None, true);
    }*/
}


void AFighterSphere::JumpToTarget(const FVector& TargetLocation)
{

    FVector BallLocation = GetActorLocation();
    FVector Direction = (TargetLocation - BallLocation);
    

  
        Direction.Normalize();

       
        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);
        FVector Start = BallLocation;
        FVector End = BallLocation - FVector(0, 0, 50);
        bool bOnGround = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

        if (bOnGround)
        {
          
            FVector Impulse = Direction * (JumpImpulse * 0.6f) + FVector(0, 0, JumpImpulse);
            BallMesh->AddImpulse(Impulse, NAME_None, true);
        }
    
}

void AFighterSphere::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        DEBUG_CHECK("PlayerChar", "LookAction", LookAction)
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFighterSphere::Look);
        DEBUG_CHECK("PlayerChar", "MoveAction", MoveAction)
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFighterSphere::Move);       
    }
}

void AFighterSphere::Move(const FInputActionValue& Value)
{
    /*MovementVector = Value.Get<FVector2D>();*/
   
}

void AFighterSphere::Die()
{
    FVector Velos = GetVelocity();
    BallVisMesh->SetVisibility(false);
    BallMesh->SetSimulatePhysics(false);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DestoyeblComp->SetSimulatePhysics(true);
    DestoyeblComp->SetVisibility(true);
      
    DestoyeblComp->CrumbleCluster(0);

  

    DestoyeblComp->ApplyBreakingLinearVelocity(0, Velos);

    
    FTimerHandle handle;    
    GetWorld()->GetTimerManager().SetTimer(
        handle,
        [this]() {Destroy();},
        10.0f,
        false
    );
    IsDie = 1;
}

void AFighterSphere::Look(const FInputActionValue& Value)
{
   /* FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(MovementVector.X);
        AddControllerPitchInput(-MovementVector.Y);
    }*/
}


void AFighterSphere::MoveToLocation(const FVector& TargetLocation)
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys) return;

    UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), TargetLocation);
    if (NavPath && NavPath->IsValid())
    {
        PathPoints = NavPath->PathPoints;
        CurrentPathIndex = 0;
    }
          
}

void AFighterSphere::MoveAlongPath(float DeltaTime)
{
    if (PathPoints.Num() == 0 || CurrentPathIndex >= PathPoints.Num()) return;

    FVector CurrentTarget = PathPoints[CurrentPathIndex];
    FVector BallLocation = GetActorLocation();
    FVector ToTarget = CurrentTarget - BallLocation;
    ToTarget.Z = 0;

    if (ToTarget.Size() < 50.f)
    {
        
        CurrentPathIndex++;
        return;
    }

  
    FVector Dir = ToTarget.GetSafeNormal();

   
    FVector Torque = FVector::CrossProduct(FVector::UpVector, Dir) * RollForce * DeltaTime;
    BallMesh->AddTorqueInRadians(Torque, NAME_None, true);
}


void AFighterSphere::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
    UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
    FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    if (Other && Other != this)
    {       
        UDamageManager* DamManager = Other->FindComponentByClass<UDamageManager>();
        if (DamManager && DamManager->GetTeamID() != MyDamageManager->GetTeamID() && DamManager->GetTeamID() != 0)
        {

            float CurrentTime = GetWorld()->GetTimeSeconds();

            if (DamageCooldowns.Contains(DamManager))
            {
                float NextTime = DamageCooldowns[DamManager];
                if (CurrentTime < NextTime)
                {                   
                    return;
                }
            }

            DamManager->TakeDamage(Damage, DamageType);
           
            DamageCooldowns.Add(DamManager, CurrentTime + DamageCooldown);
        }

    }
}

