// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Cube/Fight/DamageManager.h"


// Sets default values
ABullet::ABullet()
{
 
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = MainMesh;
	MainMesh->SetSimulatePhysics(false);
	MainMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);	

	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bShouldBounce = true;
	
	//ProjectileMovement->OnProjectileStop.AddDynamic(this, &ABullet::OnProjStop);


	
	//HitDamageType = UDamageType::StaticClass();
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ABullet::OnProjStop);
	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABullet::Shoot()
{
	//MainMesh->AddImpulse(GetActorForwardVector() * Forse, NAME_None, true);
	//SetLifeSpan(LifeTime);
//	CanStop = 1;
}

void ABullet::SetDamage(int Value)
{
	Damage = Value;
}

void ABullet::SetLifeTime(double Value)
{
	LifeTime = Value;
}

void ABullet::SetForse(double Value)
{
	Forse = Value;
}

UFUNCTION()
void ABullet::OnProjStop(const FHitResult& ImpactResult)
{
	UE_LOG(LogTemp, Log, TEXT("Projectile stopped at %s"), *ImpactResult.ImpactPoint.ToString());	
	Destroy();
}

void ABullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
	UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{	
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if (Other && Other != this)
	{
		// Здесь можно нанести урон
		UE_LOG(LogTemp, Warning, TEXT("Bullet hit %s at %s"), *Other->GetName(), *HitLocation.ToString());

		UDamageManager* DamManager = Other->FindComponentByClass<UDamageManager>();
		if (DamManager)
		{
			DamManager->TakeDamage(10);
			Destroy();
		}

		
		if (!Other->ActorHasTag("CanRebound"))
		{
			Destroy();
		}
	}
}
