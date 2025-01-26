// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Pawns/BaseTankPawn.h"
#include "EnemyTank.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API AEnemyTank : public ABaseTankPawn
{
	GENERATED_BODY()

public:
	AEnemyTank();
	void UpdateEnemyHUD(float Health);
	

protected:
	virtual void BeginPlay() override;
	void ConstructTankBarrel();
	virtual void ConstructTankHierarchy() override;
	void ConstructMissileSpawnPoint();
	void ConstructHealthHUD();
	void TargetPlayer(FVector PlayerPawnLocation);
	void AttackPlayer();
	void SetEnemyHUDWidget();
	void AwakeAfter(float Time);

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		UStaticMeshComponent* TankBarrel;
	UPROPERTY(EditAnywhere, Category = "Components | HUD", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		class UWidgetComponent* WidgetComponent;
	

	/**
	* Global Properties
	*/
	UPROPERTY(EditAnywhere,Category="Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		AActor* PlayerPawn = nullptr;
	UPROPERTY(EditAnywhere,Category="Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float TargetDetectionDistance = 2400.0f;
	UPROPERTY(EditAnywhere,Category="Spawn", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		USceneComponent* MissileSpawnPoint;
	UPROPERTY(EditAnywhere,Category="Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float FireDelayRate = 300.f;
	float FireDelayTime = 0.f;
	TSubclassOf<class UEnemyHUD> WBP_EnemyHUDClass;
		UEnemyHUD* WBP_EnemyHUD = nullptr;
	
};
