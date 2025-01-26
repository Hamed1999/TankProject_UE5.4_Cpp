// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Pawns/BaseTankPawn.h"
#include "Actors/Projectile.h"
#include "Components/HealthComponent.h"

void ABaseTankPawn::SetDefaultMissileClass()
{
	ConstructorHelpers::FClassFinder<AProjectile> MissileClassReference(TEXT("/Script/TankProject.Projectile"));
	if(MissileClassReference.Succeeded())
	{
		MissileClass = MissileClassReference.Class;
		UE_LOG(LogTemp, Log, TEXT("Missile Projectile class was found."));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Missile Projectile class was Not found!"));
}

void ABaseTankPawn::ConstructHealthComponent()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("Health"));
}

ABaseTankPawn::ABaseTankPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	 SetDefaultMissileClass();
	ConstructHealthComponent();
}

void ABaseTankPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseTankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABaseTankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector ABaseTankPawn::GetTurretWorldLocation()
{
	return TankTurret->GetComponentLocation();
}

void ABaseTankPawn::RotateTurretTowardsTarget(FVector TargetLocation)
{
	FVector TurretLocation = TankTurret->GetComponentLocation();
	FVector FixedTargetLocation = FVector(TargetLocation.X, TargetLocation.Y, TurretLocation.Z);

	FRotator NewRotation = FVector(FixedTargetLocation - TurretLocation).Rotation();
	FRotator SmoothNewRotation = FMath::Lerp(TankTurret->GetComponentRotation(), NewRotation, GetWorld()->GetDeltaSeconds() * TurretRotationSpeed);
	TankTurret->SetWorldRotation(SmoothNewRotation);
}

UStaticMesh* ABaseTankPawn::FindMesh(const TCHAR* path)
{
	return FindObjectByAddress<UStaticMesh>(path);
}

void ABaseTankPawn::ConstructTankHierarchy()
{
	ConstructRoot();
}

void ABaseTankPawn::ConstructRoot()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
}

void ABaseTankPawn::ConstructTankBody(const TCHAR* AssetPath)
{
	TankBody = CreateDefaultSubobject<UStaticMeshComponent>("Tank Body");
	// TankBody->SetupAttachment(RootComponent);
	RootComponent = TankBody;
	TankBody->SetStaticMesh(FindMesh(AssetPath));
}

void ABaseTankPawn::ConstructTankTurret(const TCHAR* AssetPath, FVector Location)
{
	TankTurret = CreateDefaultSubobject<UStaticMeshComponent>("Tank Turret");
	TankTurret->SetupAttachment(TankBody);
	TankTurret->SetRelativeLocation(Location);
	TankTurret->SetStaticMesh(FindMesh(AssetPath));
}

USceneComponent* ABaseTankPawn::ConstructProjectileSpawnPoint(UStaticMeshComponent* AttatchedTo, FVector Location,
	FRotator Rotation, const FName Name) 
{
	USceneComponent* SpawnPoint = CreateDefaultSubobject<USceneComponent>(Name);
	SpawnPoint->SetupAttachment(AttatchedTo);
	SpawnPoint->SetRelativeLocation(Location);
	SpawnPoint->SetRelativeRotation(Rotation);
	return SpawnPoint;
}

AProjectile*  ABaseTankPawn::Fire(const FTransform& Transform, TSubclassOf<AProjectile> ProjectileClass,
	bool bFireItNow)
{
	// ProjectileClass = AProjectile::StaticClass();
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Transform);
	if (bFireItNow)
		SpawnedProjectile->FireItNow();
		SpawnedProjectile->SetOwner(this);
	return SpawnedProjectile;
}
