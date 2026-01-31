#include "Enemy.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIController.h"
#include "Cube/Weapons/Weapon.h"
#include "Cube/DamageManager.h"



AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 5000.0f;
	SightConfig->LoseSightRadius = 8000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 110.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(*SightConfig->GetSenseImplementation());
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnPerceptionUpdated);

	MainMesh = GetMesh();
	
	//MyDamageManager = CreateDefaultSubobject<UDamageManager>(TEXT("DamageManager"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	FVector WeaponLocation = MainMesh->GetSocketLocation("WeaponHand");
	FRotator WeaponRotation = MainMesh->GetSocketRotation("WeaponHand");

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	HandligWeapon = GetWorld()->SpawnActor<AWeapon>(HandligWeaponType, WeaponLocation, WeaponRotation, Params);
	HandligWeapon->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "WeaponHand");

	if (AAIController* AI = Cast<AAIController>(GetController()))	
		MyAIControoler = AI;	
	else
		UE_LOG(LogTemp, Warning, TEXT("Not AI controller"));

	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (PlayerActor)
	//	MoveToLocation(PlayerActor->GetActorLocation());
	//if (bCanSeePlayer && PlayerActor)
	//{
	////	RotateToPlayer(DeltaTime);
	//	Shoot();
	//}
	// if (FVector::Dist(GetActorLocation(), CurrentDestination) < AcceptanceRadius)
	//{
	//	MoveToNextPatrolPoint();
	//}
	//if (AAIController* AI = Cast<AAIController>(GetController()))
	//{
	//	AI->StopMovement();
	//}

	if (bCanSeePlayer)
	{
		RotateToPlayer(DeltaTime);
		if (FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation()) < 1000.f)
			MyAIControoler->StopMovement();
		else
			MyAIControoler->MoveToLocation(PlayerActor->GetActorLocation());
	}
}

void AEnemy::MoveToNextPatrolPoint()
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetSys"));
		FNavLocation RandomLoc;
		if (NavSys->GetRandomReachablePointInRadius(GetActorLocation(), PatrolRadius, RandomLoc))
		{
			UE_LOG(LogTemp, Warning, TEXT("CanRandoom"));
			CurrentDestination = RandomLoc.Location;
			MyAIControoler->MoveToLocation(CurrentDestination);
			
		}
	}
}


void AEnemy::RotateToPlayer(float DeltaTime)
{
	if (!PlayerActor) return;

	FVector ToTarget = PlayerActor->GetActorLocation() - GetActorLocation();
	FRotator TargetRot = ToTarget.Rotation();
	FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, RotationSpeed);
	SetActorRotation(NewRot);

	
}

void AEnemy::Shoot()
{
	//            
	UE_LOG(LogTemp, Warning, TEXT("Bang!"));
}

void AEnemy::LosePlayer()
{
	if (!PlayerActor)
		return;
	HandligWeapon->SetAttaking(false);
	MyAIControoler->MoveToLocation(PlayerActor->GetActorLocation());
}

void AEnemy::FindPlayer()
{
	HandligWeapon->SetAttaking(true);
	LastKnownLocation = PlayerActor->GetActorLocation();
	
}

void AEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->ActorHasTag("Player")) //        ,                           
		{
			if (GetController()->LineOfSightTo(Actor))
			{
				if (!bCanSeePlayer)
				{
					bCanSeePlayer = true;
					PlayerActor = Actor;
					FindPlayer();
				}
				return;
			}
		}
	}

	//                     
	if (bCanSeePlayer)
	{
		bCanSeePlayer = false;
		LosePlayer();
	}
}