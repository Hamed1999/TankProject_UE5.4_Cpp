// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UENUM(BlueprintType)
enum class EProjectileTypes : uint8
{
	Bullet UMETA(DisplayName = "Bullet"),
	Missile UMETA(DisplayName = "Missile")
};


UCLASS()
class TANKPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,Category="Projectile Type", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		EProjectileTypes Type = EProjectileTypes::Missile;
	/**
	* Methods
	*/
	void FireItNow();
	void CustomizeProjectile(UStaticMesh* NewMesh , float NewSpeed, float DamageValue = 5.f, float ImpulseStrength = 10000.0f,float MaxSpeed = 40000.f,
		float ProjectileGravityScale = 1.f);
	void SetDamageValue(float NewDamage);
	void SetNewSpeed(float NewSpeed);
	void SetGravityScale(float NewGravityScale);

protected:
	/**
	 *Methods
	 */
	void ConstructMesh();
	void ConstructSphereCollision();
	void ConstructRadialForce();
	void SetProjectileProperties();
	void ConstructNiagaraComponent();
	void SetDefaultAudioAndVisualEffects();
	UStaticMesh* FindMesh(const TCHAR* path);
	void SimulateWindEffect();
	void HandlePlayHitSoundsAndExplosion(FVector HitLocation);
	// virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	// 	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION()
		virtual void OnHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);
	
	/**
	* Components
	**/
	UPROPERTY(EditAnywhere,Category="Components | Mesh", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		UStaticMeshComponent* ProjectileMesh = nullptr;
	UPROPERTY(EditAnywhere,Category="Components | Movement", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		class UProjectileMovementComponent* ProjectileMovement = nullptr;
	UPROPERTY(EditAnywhere,Category="Collision", BlueprintReadWrite, Meta = (AllowPrivateAccess = true));
		class USphereComponent* SphereCollision = nullptr;
	UPROPERTY(EditAnywhere,Category="Components | Explosion", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		class URadialForceComponent* RadialForce = nullptr;

	/**
	* Global Properties
	**/
	UPROPERTY(EditAnywhere,Category="Properties | Damage", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		float Damage  = 1.f;
	UPROPERTY(EditAnywhere,Category="Properties | Velocity", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		FVector CurrentVelocity = FVector(0.f);
	float Timer = 0.f;
	UPROPERTY(EditAnywhere,Category="Properties | Damage", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		TSubclassOf<class UDamageType> DamageTypeClass;
	/**
	 * Effects
	 */
	UPROPERTY(EditAnywhere,Category="Properties | Sound", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		USoundBase* FireBulletSound = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties | Sound", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		USoundBase* FireMissileSound = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties | Sound", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		USoundBase* ExplosionSound = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties | Sound", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		USoundBase* ImpactSound = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties | Sound", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		USoundAttenuation* SA_Explosion = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties | Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		class UNiagaraComponent* SmokeComponent;
	UPROPERTY(EditAnywhere,Category="Properties | Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		UParticleSystem* BulletTrace = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties | Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		class UNiagaraSystem* SmokeNiagaraSystem = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties | Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn));
		UParticleSystem* ExplosionParticle = nullptr;
};
