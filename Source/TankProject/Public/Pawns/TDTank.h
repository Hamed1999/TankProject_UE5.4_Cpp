// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TDTank.generated.h"

UCLASS()
class TANKPROJECT_API ATDTank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATDTank();
	void CreateMappingContext();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void RotateTurretTowardsTarget(FVector CursorLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DepricatedBindAxis(UInputComponent* PlayerInputComponent);
	void BindEnhancedInputActions(UInputComponent* PlayerInputComponent);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	/**
	* Methods
	*/
	void ConstructTankHierarchy();
	void ConstructMovementComponent();
	void SetDefaultInputs();
	void HandleTankPhysics();
	void ConstructRoot();
	template<class T> T* FindDefaultInputs(const TCHAR* path);
	UStaticMesh* FindMesh(const TCHAR* path);
	void ConstructTankBody();
	void ConstructTankTurret();
	void ConstructRightWeapon();
	void ConstructLeftWeapon();
	void ConstructMissiles();
	

	void ConstructSpringArm();
	void ConstructCamera();

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
		UStaticMeshComponent* TankBody;
    UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
    	UStaticMeshComponent* TankTurret;
     UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
        	UStaticMeshComponent* RightMissile;
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
		UStaticMeshComponent* LeftMissile;
	UPROPERTY(EditAnywhere, Category = "Components | Static Mesh", meta = (AllowPrivateAccess = "true"));
            	UStaticMeshComponent* RightWeapon;
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
	// UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
	// 	 UInputAction* IA_TurnCamera = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		 UInputAction* IA_ZoomCamera = nullptr;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		 UInputAction* IA_TankMode = nullptr;
		

	/**
	 *	Global Properties
	 */
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float MovementSpeed = 800.0f;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float RotationSpeed = 400.0f;
	UPROPERTY(EditAnywhere,Category="Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true));
		float TurretRotationSpeed = 2.0f;
	
};