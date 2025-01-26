// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Pawns/MultiMTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "Actors/Projectile.h"

#define InitArmLength 850.0f


void AMultiMTank::ConstructSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->AddRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	SpringArm->AddRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	ArmLength = InitArmLength;
	SpringArm->TargetArmLength = ArmLength;

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 12.0;
	SpringArm->CameraRotationLagSpeed = 3.0;
	
}

void AMultiMTank::ConstructCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void AMultiMTank::ConstructTankBody()
{
	Super::ConstructTankBody(TEXT("/Game/Mesh/SM_TankBody"));
	RootComponent->SetWorldScale3D(FVector(5.0f));
}

template<class T>
T* AMultiMTank::FindDefaultInputs(const TCHAR* path)
{
	ConstructorHelpers::FObjectFinder<T> Asset (path);
	if (Asset.Succeeded())
		return Asset.Object.Get();
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Asset Was Not Found!"));
		return nullptr;
	}
}

void AMultiMTank::ConstructTankTurret()
{
	Super::ConstructTankTurret(TEXT("/Game/Mesh/SM_MissileBarrel"), FVector(-7.556f, 0.0f, 27.095f));
}

void AMultiMTank::ConstructRightWeapon()
{
	RightWeapon = CreateDefaultSubobject<UStaticMeshComponent>("Right Weapon");
	RightWeapon->SetupAttachment(TankBody);
	RightWeapon->SetRelativeLocation(FVector(14.28f, 9.71f, 24.02f));
	
	RightWeapon->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_RightWeapon")));
}

void AMultiMTank::ConstructLeftWeapon()
{
	LeftWeapon = CreateDefaultSubobject<UStaticMeshComponent>("Left Weapon");
	LeftWeapon->SetupAttachment(TankBody);
	LeftWeapon->SetRelativeLocation(FVector(13.645f, -9.93f, 24.02f));
    	
	LeftWeapon->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_LeftWeapon")));
}

void AMultiMTank::ConstructMissiles()
{
	RightMissile = CreateDefaultSubobject<UStaticMeshComponent>(FName("Right Missile Mesh"));
	RightMissile->SetupAttachment(TankTurret);
	
	LeftMissile = CreateDefaultSubobject<UStaticMeshComponent>(FName("Left Missile Mesh"));
	LeftMissile->SetupAttachment(TankTurret);
		
	RightMissile->SetRelativeLocation(FVector(-1.75f, 3.85f, 5.54f));
	RightMissile->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f));
	LeftMissile->SetRelativeLocation(FVector(-1.75f, -4.3f, 5.54f));
	LeftMissile->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f));
	MissileMesh = FindMesh(TEXT("/Game/Mesh/SM_Missile"));
	RightMissile->SetStaticMesh(MissileMesh);
	LeftMissile->SetStaticMesh(MissileMesh);
}

void AMultiMTank::ConstructTankHierarchy()
{
	Super::ConstructTankHierarchy();
	ConstructTankBody();
	ConstructTankTurret();
	ConstructRightWeapon();
	ConstructLeftWeapon();
	ConstructMissiles();
}

void AMultiMTank::ConstructMovementComponent()
{
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(FName("Movement Component"));
}

void AMultiMTank::SetDefaultInputs()
{
	DefaultMappingContext = FindDefaultInputs<UInputMappingContext>(TEXT("/Game/Inputs/IMC_Movements"));
	IA_MForward = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_MForward"));
	IA_MRight = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_MRight"));
	IA_TurnRight = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TurnRight"));
	IA_TurnCamera =	FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TurnCamera"));
	IA_ZoomCamera = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_ZoomCamera"));
	IA_TankMode = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TankMode"));
	IA_FireBullets = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_FireBullets"));
	IA_FireMissiles = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_FireMissiles"));
}

void AMultiMTank::HandleTankPhysics()
{
	TankBody->SetSimulatePhysics(true);
	TankBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TankBody->SetAllMassScale(0.01);
	TankBody->SetMassOverrideInKg(TEXT("Masss (Kg)"),650.f,true);
	TankBody->SetLinearDamping(0.02);
	TankBody->SetAngularDamping(0.6);
}

void AMultiMTank::ConstructBulletSpawnPoints()
{
	FVector Scale{FVector(0.1f, 0.03, 0.03)};
	RightBulletSpawnPoint = ConstructProjectileSpawnPoint(RightWeapon, FVector(20.f,0.f,0.f), FRotator(0.f), FName("Right Bullet Spawn Point"));
	RightBulletSpawnPoint->SetRelativeScale3D(Scale);
	LeftBulletSpawnPoint = ConstructProjectileSpawnPoint(LeftWeapon, FVector(20.f,0.f,0.f), FRotator(0.f), FName("Left Bullet Spawn Point"));
	LeftBulletSpawnPoint->SetRelativeScale3D(Scale);
}

// Sets default values
AMultiMTank::AMultiMTank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructTankHierarchy();
	ConstructSpringArm();
	ConstructCamera();
	ConstructMovementComponent();
	SetDefaultInputs();
	ConstructBulletSpawnPoints();
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	/**
	 * For TurnRight
	 */
	this->bUseControllerRotationYaw = true;
	HandleTankPhysics();
	BulletClass = AProjectile::StaticClass();
}

void AMultiMTank::CreateMappingContext()
{
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called when the game starts or when spawned
void AMultiMTank::BeginPlay()
{
	Super::BeginPlay();
	CreateMappingContext();
}

void AMultiMTank::SetZeroPitch(UStaticMeshComponent* TargetStaticMesh)
{
	FRotator ComponentRotation = TargetStaticMesh->GetRelativeRotation();
	TargetStaticMesh->SetRelativeRotation(FRotator(0, ComponentRotation.Yaw, ComponentRotation.Roll));
}

void AMultiMTank::SetTopDownMode()
{
	PlayerController->bShowMouseCursor = true;
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	FVector CursorLocation = HitResult.ImpactPoint;
	RotateTurretTowardsTarget(CursorLocation);
	SetZeroPitch(RightWeapon);
	SetZeroPitch(LeftWeapon);
	SpringArm->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, 2*InitArmLength,0.55*GetWorld()->GetDeltaSeconds()*TurretRotationSpeed);
}

void AMultiMTank::SetRegualrMode()
{
	PlayerController->bShowMouseCursor = false;
	SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, ArmLength,0.65*GetWorld()->GetDeltaSeconds()*TurretRotationSpeed);
}

// Called every frame
void AMultiMTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerController =GetWorld()->GetFirstPlayerController();
	if(bTopDownMode)
	{
		SetTopDownMode();
	}
	else
	{
		SetRegualrMode();
	}
}

void AMultiMTank::ChangeTankMode(const struct FInputActionValue& val)
{
	bTopDownMode = !bTopDownMode;
	if(bTopDownMode)
	{
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		PlayerController->bShowMouseCursor = false;
		SpringArm->SetRelativeRotation(FRotator(-20.0, TankTurret->GetRelativeRotation().Yaw, 0));
	}
}

void AMultiMTank::DeprecatedBindAxis(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiMTank::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiMTank::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &AMultiMTank::TurnRight);
	if (!bTopDownMode)
	{
		PlayerInputComponent->BindAxis("TurnYawView", this, &AMultiMTank::TurnYawView);
		PlayerInputComponent->BindAxis("TurnPitchView", this, &AMultiMTank::TurnPitchView);
	}
	PlayerInputComponent->BindAxis("CameraDistance", this, &AMultiMTank::ZoomCamera);
	PlayerInputComponent->BindAction("FireBullets", IE_Pressed, this, &AMultiMTank::FireWeapons);
}

void AMultiMTank::BindEnhancedInputActions(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MForward, ETriggerEvent::Triggered, this, &AMultiMTank::MoveForward);
		EnhancedInputComponent->BindAction(IA_MRight, ETriggerEvent::Triggered, this, &AMultiMTank::MoveRight);
		EnhancedInputComponent->BindAction(IA_TurnRight, ETriggerEvent::Triggered, this, &AMultiMTank::TurnRight);
		if (!bTopDownMode)
		{
			EnhancedInputComponent->BindAction(IA_TurnCamera, ETriggerEvent::Triggered, this, &AMultiMTank::TurnCamera);
			EnhancedInputComponent->BindAction(IA_ZoomCamera, ETriggerEvent::Triggered, this, &AMultiMTank::ZoomCamera);
		}
		EnhancedInputComponent->BindAction(IA_TankMode, ETriggerEvent::Triggered, this, &AMultiMTank::ChangeTankMode);
		EnhancedInputComponent->BindAction(IA_FireBullets, ETriggerEvent::Triggered, this, &AMultiMTank::FireWeapons);
		EnhancedInputComponent->BindAction(IA_FireMissiles, ETriggerEvent::Triggered, this, &AMultiMTank::FireMissiles);
	}
}

void AMultiMTank::FireWeapons()
{
	AProjectile* FiredBullet;
	FiredBullet = Fire(RightBulletSpawnPoint->GetComponentTransform(), BulletClass);
	FiredBullet->SetGravityScale(0.4);
	FiredBullet->Type = EProjectileTypes::Bullet;
	FiredBullet->FireItNow();
	FiredBullet = Fire(LeftBulletSpawnPoint->GetComponentTransform(), BulletClass);
	FiredBullet->SetGravityScale(0.4);
	FiredBullet->Type = EProjectileTypes::Bullet;
	FiredBullet->FireItNow();
}

void AMultiMTank::FireMissiles()
{
	FTransform TargetSpawnTransform;
	FTimerHandle ReloadTimerHandle;
	if (bRightMissileLoaded)
	{
		TargetSpawnTransform = RightMissile->GetComponentTransform();
		RightMissile->SetStaticMesh(nullptr);
		bRightMissileLoaded = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AMultiMTank::Reload, 3.f);
	}
	else if (bLeftMissileLoaded)
	{
		TargetSpawnTransform = LeftMissile->GetComponentTransform();
		LeftMissile->SetStaticMesh(nullptr);
		bLeftMissileLoaded = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AMultiMTank::Reload, 3.f);
	}
	if(TargetSpawnTransform.IsValid())
	{
		AProjectile* ProjectedMissile = Fire(TargetSpawnTransform, MissileClass);
		ProjectedMissile->CustomizeProjectile(MissileMesh, 2000, 20, 150000);
		ProjectedMissile->FireItNow();
	}
}

void AMultiMTank::Reload()
{
	if (MissilesNum == 0) return;
	
	if(!bRightMissileLoaded)
	{
		RightMissile->SetStaticMesh(MissileMesh);
		bRightMissileLoaded = true;
	}
	else if(!bLeftMissileLoaded)
	{
		LeftMissile->SetStaticMesh(MissileMesh);
		bLeftMissileLoaded = true;
	}
	MissilesNum--;
}


void AMultiMTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// DepricatedBindAxis(PlayerInputComponent);
	BindEnhancedInputActions(PlayerInputComponent);
}

void AMultiMTank::MoveForward(float val){
	if (val !=	0.0f )
	{
		// this->AddMovementInput(GetActorForwardVector(), MovementSpeed*GetWorld()->GetDeltaSeconds()*val, true);
		FloatingPawnMovement->MoveUpdatedComponent(GetActorForwardVector() * MovementSpeed*GetWorld()->GetDeltaSeconds()*val,
			FRotator(TankBody->GetComponentRotation()), false);
	}
}

void AMultiMTank::MoveForward(const FInputActionValue& val)
{
	MoveForward(val.Get<float>());
}

void AMultiMTank::MoveRight(float val){
	if (val !=	0.0f )
	{
		this->AddMovementInput(GetActorRightVector(), MovementSpeed*GetWorld()->GetDeltaSeconds()*val, true);
		// FloatingPawnMovement->MoveUpdatedComponent(GetActorRightVector() * MovementSpeed*GetWorld()->GetDeltaSeconds()*val, FRotator(0.0f), false);
	}
}

void AMultiMTank::MoveRight(const struct FInputActionValue& val)
{
	MoveRight(val.Get<float>());
}

void AMultiMTank::TurnRight(float val){
	if (val !=	0.0f )
	{
		this->AddControllerYawInput(val);
		// TankBody->SetWorldRotation(TankBody->GetComponentRotation().Add(0.0, val, 0.0));
	}
}

void AMultiMTank::TurnRight(const struct FInputActionValue& val)
{
	TurnRight(val.Get<float>());
}

void AMultiMTank::RotateWeapons(float YawVal, float PitchVal)
{
	TankTurret->AddRelativeRotation(FRotator(0.0, YawVal, 0.0f));

	FRotator NewRotation = FRotator(RightWeapon->GetRelativeRotation().Pitch, TankTurret->GetRelativeRotation().Yaw, 0.0f);
	if (abs(NewRotation.Yaw) < 75.0f)
	{
		RightWeapon->SetRelativeRotation(NewRotation.Add(0.0f, YawVal, 0.0f));
		LeftWeapon->SetRelativeRotation(NewRotation.Add(0.0f, YawVal, 0.0f));
	}
	else if(RightWeapon->GetRelativeRotation().Pitch < -4.0f)
	{
		LeftWeapon->SetRelativeRotation(FRotator(-4.0f,RightWeapon->GetRelativeRotation().Yaw,0.0f));
		RightWeapon->SetRelativeRotation(FRotator(-4.0f,RightWeapon->GetRelativeRotation().Yaw,0.0f));
	}
	
	NewRotation = RightWeapon->GetRelativeRotation().Add(PitchVal, 0.0, 0.0);
	
	if (NewRotation.Pitch <= 23.0f && NewRotation.Pitch >= -10.0f)
	{
		RightWeapon->SetRelativeRotation(NewRotation);
		LeftWeapon->SetRelativeRotation(NewRotation);
	}
}

void AMultiMTank::TurnYawView(float val){
	if (val !=	0.0f )
	{
		SpringArm->AddRelativeRotation(FRotator(0.0f, val, 0.0f));
		RotateWeapons(val, 0.0);
	}
}

void AMultiMTank::TurnPitchView(float val){
	if (val !=	0.0f )
	{
		SpringArm->AddRelativeRotation(FRotator(val, 0.0f, 0.0f));
		RotateWeapons(0.0, val);
	}
}

void AMultiMTank::RotateSpringArm(FVector2D MouseXY)
{
	FRotator NewRotation = SpringArm->GetRelativeRotation().Add(MouseXY.Y ,MouseXY.X,0.0f);
	if (NewRotation.Pitch < -3.0f && NewRotation.Pitch > -85.0)
		SpringArm->SetRelativeRotation(NewRotation);
	else
	{
		SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch ,TankTurret->GetRelativeRotation().Yaw,0.0f));
	}
}

void AMultiMTank::RotateTurret(FVector2D MouseXY)
{
	TankTurret->AddRelativeRotation(FRotator(0.0, MouseXY.X, 0.0f));
}

void AMultiMTank::RotateWeapons(FVector2D MouseXY)
{
	FRotator NewRotation = FRotator(RightWeapon->GetRelativeRotation().Pitch, TankTurret->GetRelativeRotation().Yaw, 0.0f);
	if ( abs(NewRotation.Yaw) < 73.0f)
	{
		LeftWeapon->SetRelativeRotation(NewRotation.Add(0.0f,MouseXY.X,0.0f));
		RightWeapon->SetRelativeRotation(NewRotation.Add(0.0f,MouseXY.X,0.0f));
	}
	else if(RightWeapon->GetRelativeRotation().Pitch < -4.0f)
	{
		LeftWeapon->SetRelativeRotation(FRotator(-4.0f,RightWeapon->GetRelativeRotation().Yaw,0.0f));
		RightWeapon->SetRelativeRotation(FRotator(-4.0f,RightWeapon->GetRelativeRotation().Yaw,0.0f));
	}

	NewRotation = FRotator(RightWeapon->GetRelativeRotation().Pitch + MouseXY.Y,RightWeapon->GetRelativeRotation().Yaw,0.0f);
	if (NewRotation.Pitch <= 25.0f && NewRotation.Pitch >= -10.0f)
	{
		LeftWeapon->SetRelativeRotation(NewRotation);
		RightWeapon->SetRelativeRotation(NewRotation);
	}
}

void AMultiMTank::TurnCamera(const struct FInputActionValue& val)
{
		FVector2D MouseXY = val.Get<FVector2D>();
		if (MouseXY.X != 0.0 || MouseXY.Y != 0.0 )
		{
			RotateSpringArm(MouseXY);
			RotateTurret(MouseXY);
			RotateWeapons(MouseXY);
		}
}

void AMultiMTank::ZoomCamera(float val)
{
		float NewLength = SpringArm->TargetArmLength + ZoomSpeed*val;
		if (val !=	0.0f && NewLength <= 3000.0 && NewLength >= 300.0)
		{
			if (!bTopDownMode)
				ArmLength = NewLength;
			SpringArm->TargetArmLength = NewLength;
		}
}

void AMultiMTank::ZoomCamera(const struct FInputActionValue& val)
{
	ZoomCamera(val.Get<float>());
}
