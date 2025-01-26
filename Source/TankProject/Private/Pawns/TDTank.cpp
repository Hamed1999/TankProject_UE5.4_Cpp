// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Pawns/TDTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"

#include "InputAction.h"
#include "InputMappingContext.h"

#define InitArmLength 850.0f


void ATDTank::ConstructSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->AddRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	SpringArm->AddRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = InitArmLength;

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 12.0;
	SpringArm->CameraRotationLagSpeed = 3.0;
	
}

void ATDTank::ConstructCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void ATDTank::ConstructRoot()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	RootComponent->SetWorldScale3D(FVector(5.0f));
}

void ATDTank::ConstructTankBody()
{
	TankBody = CreateDefaultSubobject<UStaticMeshComponent>("Tank Body");
	// TankBody->SetupAttachment(RootComponent);
	RootComponent = TankBody;
	TankBody->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_TankBody")));
	TankBody->SetRelativeRotation(FRotator(0.0f));
	RootComponent->SetWorldScale3D(FVector(5.0f));
}

template<class T>
T* ATDTank::FindDefaultInputs(const TCHAR* path)
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

// template<class T>
UStaticMesh* ATDTank::FindMesh(const TCHAR* path)
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

void ATDTank::ConstructTankTurret()
{
	TankTurret = CreateDefaultSubobject<UStaticMeshComponent>("Tank Turret");
	TankTurret->SetupAttachment(TankBody);
	TankTurret->SetRelativeLocation(FVector(-7.556f, 0.0f, 27.095f));
	
	TankTurret->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_MissileBarrel")));
}

void ATDTank::ConstructRightWeapon()
{
	RightWeapon = CreateDefaultSubobject<UStaticMeshComponent>("Right Weapon");
	RightWeapon->SetupAttachment(TankBody);
	RightWeapon->SetRelativeLocation(FVector(14.28f, 9.71f, 24.02f));
	
	RightWeapon->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_RightWeapon")));
}

void ATDTank::ConstructLeftWeapon()
{
	LeftWeapon = CreateDefaultSubobject<UStaticMeshComponent>("Left Weapon");
	LeftWeapon->SetupAttachment(TankBody);
	LeftWeapon->SetRelativeLocation(FVector(13.645f, -9.93f, 24.02f));
    	
	LeftWeapon->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_LeftWeapon")));
}

void ATDTank::ConstructMissiles()
{
	RightMissile = CreateDefaultSubobject<UStaticMeshComponent>(FName("Right Missile Mesh"));
	RightMissile->SetupAttachment(TankTurret);
	
	LeftMissile = CreateDefaultSubobject<UStaticMeshComponent>(FName("Left Missile Mesh"));
	LeftMissile->SetupAttachment(TankTurret);
		
	RightMissile->SetRelativeLocation(FVector(-1.75f, 3.85f, 5.54f));
	RightMissile->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f));
	LeftMissile->SetRelativeLocation(FVector(-1.75f, -4.3f, 5.54f));
	LeftMissile->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f));

	RightMissile->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_Missile")));
	LeftMissile->SetStaticMesh(RightMissile->GetStaticMesh());
}

void ATDTank::ConstructTankHierarchy()
{
	ConstructRoot();
	ConstructTankBody();
	ConstructTankTurret();
	ConstructRightWeapon();
	ConstructLeftWeapon();
	ConstructMissiles();
}

void ATDTank::ConstructMovementComponent()
{
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(FName("Movement Component"));
}

void ATDTank::SetDefaultInputs()
{
	DefaultMappingContext = FindDefaultInputs<UInputMappingContext>(TEXT("/Game/Inputs/IMC_Movements"));
	IA_MForward = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_MForward"));
	IA_MRight = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_MRight"));
	IA_TurnRight = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TurnRight"));
	// IA_TurnCamera =	FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TurnCamera"));
	IA_ZoomCamera = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_ZoomCamera"));
	IA_TankMode = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TankMode"));
}

void ATDTank::HandleTankPhysics()
{
	TankBody->SetSimulatePhysics(true);
	TankBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TankBody->SetAllMassScale(0.01);
	TankBody->SetMassOverrideInKg(TEXT("Masss (Kg)"),650.f,true);
	TankBody->SetLinearDamping(0.02);
	TankBody->SetAngularDamping(0.6);
}

// Sets default values
ATDTank::ATDTank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructTankHierarchy();
	ConstructSpringArm();
	ConstructCamera();
	ConstructMovementComponent();
	SetDefaultInputs();
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	/**
	 * For TurnRight
	 */
	this->bUseControllerRotationYaw = false;
	HandleTankPhysics();
}

void ATDTank::CreateMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called when the game starts or when spawned
void ATDTank::BeginPlay()
{
	Super::BeginPlay();
	CreateMappingContext();
}

void ATDTank::RotateTurretTowardsTarget(FVector TargetLocation)
{
	FVector TurretLocation = TankTurret->GetComponentLocation();
	FVector FixedTargetLocation = FVector(TargetLocation.X, TargetLocation.Y, TurretLocation.Z);

	FRotator NewRotation = FVector(FixedTargetLocation - TurretLocation).Rotation();
	FRotator SmoothNewRotation = FMath::Lerp(TankTurret->GetComponentRotation(), NewRotation, GetWorld()->GetDeltaSeconds() * TurretRotationSpeed);
	TankTurret->SetWorldRotation(SmoothNewRotation);
}

// Called every frame
void ATDTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UWorld* World = GetWorld();
	APlayerController* PlayerController =GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	FVector CursorLocation = HitResult.ImpactPoint;
	RotateTurretTowardsTarget(CursorLocation);
	
	
}

void ATDTank::DepricatedBindAxis(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &ATDTank::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATDTank::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &ATDTank::TurnRight);
	PlayerInputComponent->BindAxis("TurnYawView", this, &ATDTank::TurnYawView);
	PlayerInputComponent->BindAxis("TurnPitchView", this, &ATDTank::TurnPitchView);
	PlayerInputComponent->BindAxis("CameraDistance", this, &ATDTank::ZoomCamera);
}

void ATDTank::BindEnhancedInputActions(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MForward, ETriggerEvent::Triggered, this, &ATDTank::MoveForward);
		EnhancedInputComponent->BindAction(IA_MRight, ETriggerEvent::Triggered, this, &ATDTank::MoveRight);
		EnhancedInputComponent->BindAction(IA_TurnRight, ETriggerEvent::Triggered, this, &ATDTank::TurnRight);
		// EnhancedInputComponent->BindAction(IA_TurnCamera, ETriggerEvent::Triggered, this, &ATDTank::TurnCamera);
		EnhancedInputComponent->BindAction(IA_ZoomCamera, ETriggerEvent::Triggered, this, &ATDTank::ZoomCamera);
		EnhancedInputComponent->BindAction(IA_TankMode, ETriggerEvent::Triggered, this, &ATDTank::ChangeTankMode);
	}
}

// Called to bind functionality to input
void ATDTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// DepricatedBindAxis(PlayerInputComponent);
	BindEnhancedInputActions(PlayerInputComponent);
}

void ATDTank::MoveForward(float val){
	if (val !=	0.0f )
	{
		this->AddMovementInput(GetActorForwardVector(), MovementSpeed*GetWorld()->GetDeltaSeconds()*val, true);
		// FloatingPawnMovement->MoveUpdatedComponent(GetActorForwardVector() * MovementSpeed*GetWorld()->GetDeltaSeconds()*val, FRotator(0.0f), true);
	}
}

void ATDTank::MoveForward(const FInputActionValue& val)
{
	MoveForward(val.Get<float>());
}

void ATDTank::MoveRight(float val){
	if (val !=	0.0f )
	{
		this->AddMovementInput(GetActorRightVector(), MovementSpeed*GetWorld()->GetDeltaSeconds()*val, true);
		// FloatingPawnMovement->MoveUpdatedComponent(GetActorRightVector() * MovementSpeed*GetWorld()->GetDeltaSeconds()*val, FRotator(0.0f), true);
	}
}

void ATDTank::MoveRight(const struct FInputActionValue& val)
{
	MoveRight(val.Get<float>());
}

void ATDTank::TurnRight(float val){
	if (val !=	0.0f )
	{
		// this->AddControllerYawInput(val);
		TankBody->SetWorldRotation(TankBody->GetComponentRotation().Add(0.0, val, 0.0));
	}
}

void ATDTank::TurnRight(const struct FInputActionValue& val)
{
	TurnRight(val.Get<float>());
}

void ATDTank::RotateWeapons(float YawVal, float PitchVal)
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

void ATDTank::TurnYawView(float val){
	if (val !=	0.0f )
	{
		SpringArm->AddRelativeRotation(FRotator(0.0f, val, 0.0f));
		RotateWeapons(val, 0.0);
	}
}

void ATDTank::TurnPitchView(float val){
	if (val !=	0.0f )
	{
		SpringArm->AddRelativeRotation(FRotator(val, 0.0f, 0.0f));
		RotateWeapons(0.0, val);
	}
}

void ATDTank::RotateSpringArm(FVector2D MouseXY)
{
	FRotator NewRotation = SpringArm->GetRelativeRotation().Add(MouseXY.Y ,MouseXY.X,0.0f);
	if (NewRotation.Pitch < -3.0f && NewRotation.Pitch > -85.0)
		SpringArm->SetRelativeRotation(NewRotation);
	else
	{
		SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch ,TankTurret->GetRelativeRotation().Yaw,0.0f));
	}
}

void ATDTank::RotateTurret(FVector2D MouseXY)
{
	TankTurret->AddRelativeRotation(FRotator(0.0, MouseXY.X, 0.0f));
}

void ATDTank::RotateWeapons(FVector2D MouseXY)
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

void ATDTank::TurnCamera(const struct FInputActionValue& val)
{
	FVector2D MouseXY = val.Get<FVector2D>();
	if (MouseXY.X != 0.0 || MouseXY.Y != 0.0 )
	{
		RotateSpringArm(MouseXY);
		RotateTurret(MouseXY);
		RotateWeapons(MouseXY);
	}
	
		
}

void ATDTank::ZoomCamera(float val){
	float NewLength = SpringArm->TargetArmLength + ZoomSpeed*val;
	if (val !=	0.0f && NewLength <= 3000.0 && NewLength >= 300.0)
	{
		SpringArm->TargetArmLength = NewLength;
	}
}

void ATDTank::ZoomCamera(const struct FInputActionValue& val)
{
	ZoomCamera(val.Get<float>());
}

void ATDTank::ChangeTankMode(const struct FInputActionValue& val)
{
}
