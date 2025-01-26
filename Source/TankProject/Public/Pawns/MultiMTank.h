// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Pawns/BaseTankPawn.h"
#include "MultiMTank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Health);

UCLASS()
class TANKPROJECT_API AMultiMTank : public ABaseTankPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMultiMTank();
	/**
	 * Delegates the updated Health value to the health progress bar
	 */
	UPROPERTY(BlueprintAssignable)
		FOnHealthChangedSignature OnHealthChanged;
	void SetTopDownMode();
protected:
	virtual void BeginPlay() override;
	void SetRegualrMode();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	/**
	* Methods
	*/
	virtual void ConstructTankHierarchy() override;
	void ConstructTankBody();
	void ConstructTankTurret();
	void ConstructMovementComponent();
	void SetDefaultInputs();
	void HandleTankPhysics();
	void ConstructBulletSpawnPoints();
	template<class T> T* FindDefaultInputs(const TCHAR* path);
	void ConstructRightWeapon();
	void ConstructLeftWeapon();
	void ConstructMissiles();
	void ConstructSpringArm();
	void ConstructCamera();
	void DeprecatedBindAxis(UInputComponent* PlayerInputComponent);
	void BindEnhancedInputActions(UInputComponent* PlayerInputComponent);
	void FireWeapons();
	void FireMissiles();
	void Reload();
	void SetZeroPitch(UStaticMeshComponent* TargetStaticMesh);
	void CreateMappingContext();
	/**
	 * This Function Moves Tank Forward or Backward
	 * @param val (float)
	 */
	void MoveForward(float val);
	void MoveForward(const struct FInputActionValue& val);
	void MoveRight(float val);
	void MoveRight(const struct FInputActionValue& val);
	void TurnRight(float val);
	void TurnRight(const struct FInputActionValue& val);
	void RotateWeapons(float YawVal, float PitchVal);
	void TurnYawView(float val);
	void TurnPitchView(float val);
	void RotateSpringArm(FVector2D MouseXY);
	void RotateTurret(FVector2D MouseXY);
	void RotateWeapons(FVector2D MouseXY);
	void TurnCamera(const struct FInputActionValue& val);
	void ZoomCamera(float val);
	void ZoomCamera(const struct FInputActionValue& val);
	void ChangeTankMode(const struct FInputActionValue& val);
	/**
	 * Properties | Staic Mesh Components
	 */
     UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
        	UStaticMeshComponent* RightMissile;
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
		UStaticMeshComponent* LeftMissile;
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
		UStaticMeshComponent* RightWeapon;
	UStaticMesh* MissileMesh;
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
		UStaticMeshComponent* LeftWeapon;
	UPROPERTY(EditAnywhere, Category = "Components | Arm", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Components | Camera", meta = (AllowPrivateAccess = "true"));
		class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Components | Camera", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		float ZoomSpeed = 35.0f;
	
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	class UFloatingPawnMovement* FloatingPawnMovement = nullptr;

	/**
	 * For Input Mapping Context
	 *
	 */
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		class UInputMappingContext* DefaultMappingContext = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
	    class UInputAction* IA_MForward = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		 UInputAction* IA_MRight = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		 UInputAction* IA_TurnRight = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		 UInputAction* IA_TurnCamera = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		 UInputAction* IA_ZoomCamera = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		 UInputAction* IA_TankMode = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		UInputAction* IA_FireBullets = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		UInputAction* IA_FireMissiles = nullptr;
		

	/**
	 *	Global Properties
	 */
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float MovementSpeed = 800.0f;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float RotationSpeed = 400.0f;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		bool bTopDownMode = false;
	APlayerController* PlayerController;
	UPROPERTY(EditAnywhere,Category="Spawn", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		USceneComponent* RightBulletSpawnPoint;
	UPROPERTY(EditAnywhere,Category="Spawn", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		USceneComponent* LeftBulletSpawnPoint;
	UPROPERTY(EditAnywhere,Category="Fire | Class", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		TSubclassOf<AProjectile> BulletClass;
	bool bRightMissileLoaded =  true;
	bool bLeftMissileLoaded =  true;
	UPROPERTY(EditAnywhere,Category="Fire", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		int MissilesNum = 48;
	float ArmLength;
};