// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Actors/Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/MultiMTank.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#define InitSpeed 2000.f

void AProjectile::ConstructMesh()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetStaticMesh(FindMesh(TEXT("/Game/ProjectileMesh/Projectile")));
	
}

void AProjectile::ConstructSphereCollision()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SphereCollision->SetRelativeLocation(FVector(30.0, 0.0f, 0.0f));
	SphereCollision->SetSphereRadius(10.0);
	SphereCollision->SetupAttachment(ProjectileMesh);
}

void AProjectile::ConstructRadialForce()
{
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("Radial Force");
	RadialForce->SetupAttachment(ProjectileMesh);
	RadialForce->Radius = 350.f;
	RadialForce->ImpulseStrength = 2000.f;
	RadialForce->AddObjectTypeToAffect(EObjectTypeQuery::ObjectTypeQuery1);
	RadialForce->AddObjectTypeToAffect(EObjectTypeQuery::ObjectTypeQuery2);
}

void AProjectile::SetProjectileProperties()
{
	ProjectileMovement->InitialSpeed = InitSpeed;
	CurrentVelocity = FVector(InitSpeed, 0.f, 0.f);
	ProjectileMovement->MaxSpeed = 40000.f;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMesh->SetCollisionProfileName(FName("NoCollision"));
	// ProjectileMesh->SetSimulatePhysics(false);
}


void AProjectile::ConstructNiagaraComponent()
{
	SmokeComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("Niagara System Component"));
	SmokeComponent->SetupAttachment(ProjectileMesh);
	SmokeComponent->SetRelativeRotation(FRotator(0, 180.f, 0));
	SmokeComponent->SetRelativeLocation(FVector(-17.3f, 0.0f, 0.0f));
	SmokeComponent->bAutoActivate = false;
}

void AProjectile::SetDefaultAudioAndVisualEffects()
{
	ABaseTankPawn* BasePawn = Cast<ABaseTankPawn>(ABaseTankPawn::StaticClass());
	FireBulletSound = BasePawn->FindObjectByAddress<USoundBase>(TEXT("/Game/MilitaryWeapDark/Sound/Rifle/RifleB_Fire_Cue"));
	FireMissileSound = BasePawn->FindObjectByAddress<USoundBase>(TEXT("/Game/MilitaryWeapDark/Sound/RocketLauncher/RocketLauncherB_Fire_Cue"));
	ExplosionSound = BasePawn->FindObjectByAddress<USoundBase>(TEXT("/Game/Sounds/SC_Explosion2"));
	ImpactSound = BasePawn->FindObjectByAddress<USoundBase>(TEXT("/Game/MilitaryWeapDark/Sound/Rifle/Rifle_Whip_Cue"));
	SA_Explosion = BasePawn->FindObjectByAddress<USoundAttenuation>(TEXT("/Game/Sounds/SA_Explosion"));
	SmokeNiagaraSystem =  BasePawn->FindObjectByAddress<UNiagaraSystem>(TEXT("/Game/RocketThrusterExhaustFX/FX/NS_RocketExhaust_Plume_2"));
	SmokeComponent->SetAsset(SmokeNiagaraSystem,true);
	BulletTrace = BasePawn->FindObjectByAddress<UParticleSystem>(TEXT("/Game/MilitaryWeapDark/FX/P_AssaultRifle_Tracer_01"));
	ExplosionParticle = BasePawn->FindObjectByAddress<UParticleSystem>(TEXT("/Game/VFX/P_Explosion"));
}

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructMesh();
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	SetProjectileProperties();
	ConstructSphereCollision();
	ConstructRadialForce();
	ConstructNiagaraComponent();
	ProjectileMesh->SetCollisionProfileName(FName("NoCollision"));
	ProjectileMovement->bAutoActivate = false;
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	SetDefaultAudioAndVisualEffects();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SimulateWindEffect();

}

UStaticMesh* AProjectile::FindMesh(const TCHAR* path)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> Asset (path);
	if (Asset.Succeeded())
		return Asset.Object;
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Asset Was Not Found!"));
		return nullptr;
	}
}

void AProjectile::SimulateWindEffect()
{
	Timer += GetWorld()->GetDeltaSeconds();
	FVector PurturbationVel = FVector(0.f,  3*cos(6*Timer), 3*sin(6*Timer));
	ProjectileMovement->SetVelocityInLocalSpace(CurrentVelocity + PurturbationVel);
}

void AProjectile::HandlePlayHitSoundsAndExplosion(FVector HitLocation)
{
	if (Type == EProjectileTypes::Bullet)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, HitLocation,
		                                       FRotator::ZeroRotator, 1,1,0,SA_Explosion);
	}
	else if (Type == EProjectileTypes::Missile)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, HitLocation,
		                                       FRotator::ZeroRotator, 1,1,0,SA_Explosion);
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, HitLocation);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetOwner() == OtherActor) return;
	
	HandlePlayHitSoundsAndExplosion(GetActorLocation());
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageTypeClass);	
	Destroy();
}

// void AProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
// 	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
// {
// 	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
// 	UE_LOG(LogTemp, Error, TEXT("Overlaped with %s"), *Other->GetName());
// }

// void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
// {
// 	Super::NotifyActorBeginOverlap(OtherActor);
//
// 	UE_LOG(LogTemp, Error, TEXT("Overlaped with %s"), *OtherActor->GetName());
// 	Destroy();
// }

void AProjectile::FireItNow()
{
	ProjectileMovement->Activate();
	ProjectileMesh->SetCollisionProfileName(FName("BlockAll"));
	if (Type == EProjectileTypes::Bullet)
	{
		UGameplayStatics::SpawnSoundAttached(FireBulletSound, ProjectileMesh);
		SmokeNiagaraSystem = nullptr;
		UGameplayStatics::SpawnEmitterAttached(BulletTrace, ProjectileMesh);
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(FireMissileSound, ProjectileMesh);
		SmokeComponent->Activate(true);
	}
	
}

void AProjectile::CustomizeProjectile(UStaticMesh* NewMesh, float NewSpeed, float DamageValue, float ImpulseStrength,
	float MaxSpeed, float ProjectileGravityScale)
{
	ProjectileMesh->SetStaticMesh(NewMesh);
	ProjectileMovement->SetVelocityInLocalSpace(FVector(NewSpeed, 0.0f, 0.0f));
	Damage = DamageValue;
	RadialForce->ImpulseStrength = ImpulseStrength;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->ProjectileGravityScale = ProjectileGravityScale;
}

void AProjectile::SetDamageValue(float NewDamage)
{
	Damage = NewDamage;
}

void AProjectile::SetNewSpeed(float NewSpeed)
{
	CurrentVelocity = FVector(NewSpeed, 0.0f, 0.0f);
	ProjectileMovement->SetVelocityInLocalSpace(CurrentVelocity);
}

void AProjectile::SetGravityScale(float NewGravityScale)
{
	ProjectileMovement->ProjectileGravityScale = NewGravityScale;
}
