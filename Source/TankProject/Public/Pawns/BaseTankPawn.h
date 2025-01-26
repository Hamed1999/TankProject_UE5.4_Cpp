// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseTankPawn.generated.h"

UCLASS()
class TANKPROJECT_API ABaseTankPawn : public APawn
{
	GENERATED_BODY()

public:
	ABaseTankPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FVector GetTurretWorldLocation();
	template <class T> T* FindObjectByAddress (const TCHAR* AssetPath)
	{
		if (AssetPath == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AssetPath is empty!"));
			return nullptr;
		}
		ConstructorHelpers::FObjectFinder<T> AssetRef(AssetPath);
		if (AssetRef.Succeeded())
		{
			return AssetRef.Object;
		}
		UE_LOG(LogTemp, Error, TEXT("Failed to load asset at path: %s"), AssetPath);
		return nullptr;
	}

protected:
	/**
	* Methods
	*/
	void RotateTurretTowardsTarget(FVector TargetLocation);
	UStaticMesh* FindMesh(const TCHAR* path);
	virtual void ConstructTankHierarchy();
	virtual void ConstructRoot();
	void ConstructTankBody(const TCHAR* AssetPath);
	void SetDefaultMissileClass();
	void ConstructHealthComponent();
	void ConstructTankTurret(const TCHAR* AssetPath, FVector Location);
	USceneComponent* ConstructProjectileSpawnPoint(UStaticMeshComponent* AttatchedTo, FVector Location, FRotator Rotation, const FName Name);
	class AProjectile* Fire(const FTransform& Transform, TSubclassOf<AProjectile> ProjectileClass,
		bool bFireItNow = false);
	/**
	 * Properties | Staic Mesh Components
	 */
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		UStaticMeshComponent* TankBody;
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		UStaticMeshComponent* TankTurret;
	UPROPERTY(EditAnywhere, Category = "Components | Health", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		class UHealthComponent* HealthComponent;
	/**
	 *	Global Properties
	 */
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float TurretRotationSpeed = 2.0f;
	UPROPERTY(EditAnywhere,Category="Fire | Class", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		TSubclassOf<class AProjectile> MissileClass;
	
};
