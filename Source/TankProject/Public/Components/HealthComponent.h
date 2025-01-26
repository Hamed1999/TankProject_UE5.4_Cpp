// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool CanFire();
	void UpdateEnemiesNumber();
	void DestroyPlayer();
	void DestroyEnemy();
	void PlayExplosionSound();
	void SpawnExplosionEffects();

private:
	/**
	 * Methods
	 */
	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
			AController* Instigator, AActor* DamageCauser);
	void HandleDeath();
	void SetDefaultAudioAndParticlesEffects();
	void Destroy();
	/**
	 * Global Properties
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Properties",Meta=(AllowPrivateAccess=true, ClampMin = "0"))
		float DefaultHealth = 100.0f;
	float CurrentHealth;
	bool bIsDead = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Destroy | Effects",Meta=(AllowPrivateAccess=true, ExposeOnSpawn=true));
		USoundBase* ExplosionSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Destroy | Effects",Meta=(AllowPrivateAccess=true, ExposeOnSpawn=true));
		USoundAttenuation* SA_Explosion;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Destroy | Effects",Meta=(AllowPrivateAccess=true, ExposeOnSpawn=true));
		TArray<UParticleSystem*> ExplosionParticleSystems;
	class ATGameMode* TGameMode = nullptr;
};
