// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Components/HealthComponent.h"

#include "Pawns/EnemyTank.h"
#include "Pawns/MultiMTank.h"
#include "Particles/ParticleSystem.h"
#include "Pawns/BaseTankPawn.h"
#include "Kismet/GameplayStatics.h"
#include "TGameMode.h"

void UHealthComponent::SetDefaultAudioAndParticlesEffects()
{
	if (ABaseTankPawn* BaseTankPawn = Cast<ABaseTankPawn>(GetOwner()))
	{
		ExplosionSound = BaseTankPawn->FindObjectByAddress<USoundBase>(TEXT("/Game/Sounds/SC_Explosion"));
		SA_Explosion = BaseTankPawn->FindObjectByAddress<USoundAttenuation>(TEXT("/Game/Sounds/SA_Explosion"));
		ExplosionParticleSystems.Init(nullptr, 5);
		int ParticlesArraySize = ExplosionParticleSystems.Num();
		TArray<const TCHAR*> AssetPaths{
			TEXT("/Game/VFX/Particles/P_Explosion"),
			TEXT("/Game/VFX/Particles/P_Fire"),
			TEXT("/Game/VFX/Particles/P_Smoke"),
			TEXT("/Game/VFX/Particles/P_Steam_Lit"),
			TEXT("/Game/VFX/Particles/P_PlayerInFire")
		};
		if (ParticlesArraySize != AssetPaths.Num()) return;
		for (int i = 0; i < ParticlesArraySize; i++)
		{
			ExplosionParticleSystems[i] = BaseTankPawn->FindObjectByAddress<UParticleSystem>(AssetPaths[i]);
		}
	}
}

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetDefaultAudioAndParticlesEffects();
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = DefaultHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	TGameMode = Cast<ATGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UHealthComponent::CanFire()
{
	return !bIsDead;
}


void UHealthComponent::UpdateEnemiesNumber()
{
	TGameMode->DecreaseAlliveEnemiesNum();
}

void UHealthComponent::DestroyPlayer()
{
	TGameMode->GameOver();
}

void UHealthComponent::DestroyEnemy()
{
	GetOwner()->Destroy();
}

void UHealthComponent::Destroy()
{
	FTimerHandle DestroyTimerHandle;
	if (Cast<AEnemyTank>(GetOwner()))
	{
		UpdateEnemiesNumber();
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &UHealthComponent::DestroyEnemy, 5.f);
	}
	else if (GetOwner()->GetInstigatorController())
	{
		// Cast<AMultiMTank>(GetOwner())->SetTopDownMode();
		GetWorld()->GetFirstPlayerController()->StartSpectatingOnly();
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &UHealthComponent::DestroyPlayer, 7.f);
	}
}

void UHealthComponent::PlayExplosionSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetOwner()->GetActorLocation(),
	                                      20.f, 1, 0, SA_Explosion);
}

void UHealthComponent::SpawnExplosionEffects()
{
	for (auto ExplosionParticle : ExplosionParticleSystems)
	{
		UGameplayStatics::SpawnEmitterAttached(ExplosionParticle, GetOwner()->GetRootComponent());
	}
}

void UHealthComponent::HandleDeath()
{
	bIsDead = true;
	PlayExplosionSound();
	SpawnExplosionEffects();
	Destroy();
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                  AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0 || bIsDead) return;
	CurrentHealth -= Damage;
	if (AEnemyTank* EnemyTank = Cast<AEnemyTank>(GetOwner()))
	{
		EnemyTank->UpdateEnemyHUD(CurrentHealth);
	}
	else if (AMultiMTank* MultiMTank = Cast<AMultiMTank>(GetOwner()))
	{
		MultiMTank->OnHealthChanged.Broadcast(CurrentHealth);
	}
	if (CurrentHealth <= 0)
		HandleDeath();
}

