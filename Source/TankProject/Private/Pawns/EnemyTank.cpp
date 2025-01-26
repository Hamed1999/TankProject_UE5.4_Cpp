// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Pawns/EnemyTank.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Projectile.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Pawns/MultiMTank.h"
#include "Widgets/EnemyHUD.h"

void AEnemyTank::ConstructTankBarrel()
{
	TankBarrel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Tank Barrel"));
	TankBarrel->SetupAttachment(TankTurret);
	TankBarrel->SetRelativeLocation(FVector(0.0f, 0.0f, 171.5f));
	TankBarrel->SetStaticMesh(FindMesh(TEXT("/Game/EnemyTankMesh/EnemyTankBarrel")));
}

void AEnemyTank::ConstructTankHierarchy()
{
	Super::ConstructTankHierarchy();
	ConstructTankBody(TEXT("/Game/EnemyTankMesh/EnemyTankBody"));
	ConstructTankTurret(TEXT("/Game/EnemyTankMesh/EnemyTankTurret"), FVector(0.0f));
	ConstructTankBarrel();
}

void AEnemyTank::ConstructMissileSpawnPoint()
{
	MissileSpawnPoint = ConstructProjectileSpawnPoint(TankBarrel, FVector(160.f, 0.f, 0.f), FRotator(0.0f), FName("Missile Spawn Point"));
	MissileSpawnPoint->SetRelativeScale3D(FVector(0.45));
}

void AEnemyTank::ConstructHealthHUD()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("Health HUD"));
	WidgetComponent->SetupAttachment(TankTurret);
	WidgetComponent->SetRelativeLocation(FVector(0.f,0.f,268.f));
	WidgetComponent->SetDrawSize(FVector2D(350.f,35.f));
	ConstructorHelpers::FClassFinder<UEnemyHUD> WBP_EnemyHUDRef(TEXT("/Game/UI/Widgets/WBP_EnemyHUD"));
	if (WBP_EnemyHUDRef.Succeeded())
	{
		WBP_EnemyHUDClass = WBP_EnemyHUDRef.Class;
		UE_LOG(LogTemp, Log, TEXT("WBP_EnemyHUD reference was found."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WBP_EnemyHUD was Not found!"));
		return;
	}
	WidgetComponent->SetWidgetClass(WBP_EnemyHUDClass);
}

AEnemyTank::AEnemyTank()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructTankHierarchy();
	TurretRotationSpeed = 3.0;
	ConstructMissileSpawnPoint();
	ConstructHealthHUD();
}

void AEnemyTank::UpdateEnemyHUD(float Health)
{
	if (WBP_EnemyHUD)
	{
		WBP_EnemyHUD->UpdateHealth(Health);
	}
}

void AEnemyTank::SetEnemyHUDWidget()
{
	if (WidgetComponent && IsValid(WidgetComponent->GetWidget()))
	{
		WBP_EnemyHUD = Cast<UEnemyHUD>(WidgetComponent->GetWidget());
	}
}

void AEnemyTank::AwakeAfter(float Time)
{
	FTimerHandle TickEnablingTimerHandle;
	FTimerDelegate TickEnablingTimerDelegate;
	TickEnablingTimerDelegate.BindLambda([&]()
	{
		SetActorTickEnabled(true);
	});
	GetWorldTimerManager().SetTimer(TickEnablingTimerHandle, TickEnablingTimerDelegate, Time, false);
}

void AEnemyTank::BeginPlay()
{
	Super::BeginPlay();
	SetEnemyHUDWidget();
	SetActorTickEnabled(false);
	AwakeAfter(3.5);
}

void AEnemyTank::TargetPlayer(FVector PlayerPawnLocation)
{
	RotateTurretTowardsTarget(PlayerPawnLocation);
	FVector BarrelLocation = TankBarrel->GetComponentLocation();
	FRotator NewRotation = FVector(PlayerPawnLocation - TankBarrel->GetComponentLocation()).Rotation();
	NewRotation = FRotator(NewRotation.Pitch, TankBarrel->GetComponentRotation().Yaw, 0.0f);
	FRotator SmoothNewRotation = FMath::Lerp(TankBarrel->GetComponentRotation(), NewRotation, GetWorld()->GetDeltaSeconds() * 1.5 * TurretRotationSpeed);
	TankBarrel->SetWorldRotation(SmoothNewRotation);
}

void AEnemyTank::AttackPlayer()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	if (Cast<AMultiMTank>(PlayerPawn) && HealthComponent->CanFire())
	{
		FVector PlayerPawnLocation = Cast<ABaseTankPawn>(PlayerPawn)->GetTurretWorldLocation();
		if (FVector::Dist(PlayerPawnLocation, this->GetActorLocation()) <= TargetDetectionDistance)
		{
			TargetPlayer(PlayerPawnLocation);
			if (FireDelayTime >= FireDelayRate * DeltaTime)
			{
				AProjectile* Projected = Fire(MissileSpawnPoint->GetComponentTransform(), MissileClass, true);
				Projected->SetGravityScale(0.1);
				Projected->SetDamageValue(15.f);
				FireDelayTime = 0;
			}
			FireDelayTime += DeltaTime;
		}
		else
			FireDelayTime = 0;
	}
}

void AEnemyTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	AttackPlayer();
}