// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Pawns/FighterTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"

#include "InputAction.h"
#include "InputMappingContext.h"

#define InitArmLength 850.0f


void AFighterTank::ConstructSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->AddRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	SpringArm->AddRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = InitArmLength;

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 12.0;
	SpringArm->CameraRotationLagSpeed = 3.0;
	
}

void AFighterTank::ConstructCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void AFighterTank::ConstructRoot()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	RootComponent->SetWorldScale3D(FVector(5.0f));
}

void AFighterTank::ConstructTankBody()
{
	TankBody = CreateDefaultSubobject<UStaticMeshComponent>("Tank Body");
	// TankBody->SetupAttachment(RootComponent);
	RootComponent = TankBody;
	TankBody->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_TankBody")));
	TankBody->SetRelativeRotation(FRotator(0.0f));
	RootComponent->SetWorldScale3D(FVector(5.0f));
}

template<class T>
T* AFighterTank::FindDefaultInputs(const TCHAR* path)
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
UStaticMesh* AFighterTank::FindMesh(const TCHAR* path)
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

void AFighterTank::ConstructTankTurret()
{
	TankTurret = CreateDefaultSubobject<UStaticMeshComponent>("Tank Turret");
	TankTurret->SetupAttachment(TankBody);
	TankTurret->SetRelativeLocation(FVector(-7.556f, 0.0f, 27.095f));
	
	TankTurret->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_MissileBarrel")));
}

void AFighterTank::ConstructRightWeapon()
{
	RightWeapon = CreateDefaultSubobject<UStaticMeshComponent>("Right Weapon");
	RightWeapon->SetupAttachment(TankBody);
	RightWeapon->SetRelativeLocation(FVector(14.28f, 9.71f, 24.02f));
	
	RightWeapon->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_RightWeapon")));
}

void AFighterTank::ConstructLeftWeapon()
{
	LeftWeapon = CreateDefaultSubobject<UStaticMeshComponent>("Left Weapon");
	LeftWeapon->SetupAttachment(TankBody);
	LeftWeapon->SetRelativeLocation(FVector(13.645f, -9.93f, 24.02f));
    	
	LeftWeapon->SetStaticMesh(FindMesh(TEXT("/Game/Mesh/SM_LeftWeapon")));
}

void AFighterTank::ConstructMissiles()
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

void AFighterTank::ConstructTankHierarchy()
{
	ConstructRoot();
	ConstructTankBody();
	ConstructTankTurret();
	ConstructRightWeapon();
	ConstructLeftWeapon();
	ConstructMissiles();
}

void AFighterTank::ConstructMovementComponent()
{
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(FName("Movement Component"));
}

void AFighterTank::SetDefaultInputs()
{
	DefaultMappingContext = FindDefaultInputs<UInputMappingContext>(TEXT("/Game/Inputs/IMC_Movements"));
	IA_MForward = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_MForward"));
	IA_MRight = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_MRight"));
	IA_TurnRight = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TurnRight"));
	IA_TurnCamera =	FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TurnCamera"));
	IA_ZoomCamera = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_ZoomCamera"));
	IA_TankMode = FindDefaultInputs<UInputAction>(TEXT("/Game/Inputs/IA_TankMode"));
}

void AFighterTank::HandleTankPhysics()
{
	TankBody->SetSimulatePhysics(true);
	TankBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TankBody->SetAllMassScale(0.01);
	TankBody->SetMassOverrideInKg(TEXT("Masss (Kg)"),650.f,true);
	TankBody->SetLinearDamping(0.02);
	TankBody->SetAngularDamping(0.6);
}

// Sets default values
AFighterTank::AFighterTank()
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

void AFighterTank::CreateMappingContext()
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
void AFighterTank::BeginPlay()
{
	Super::BeginPlay();
	CreateMappingContext();
}

// Called every frame
void AFighterTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFighterTank::DepricatedBindAxis(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &AFighterTank::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFighterTank::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &AFighterTank::TurnRight);
	PlayerInputComponent->BindAxis("TurnYawView", this, &AFighterTank::TurnYawView);
	PlayerInputComponent->BindAxis("TurnPitchView", this, &AFighterTank::TurnPitchView);
	PlayerInputComponent->BindAxis("CameraDistance", this, &AFighterTank::ZoomCamera);
}

void AFighterTank::BindEnhancedInputActions(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MForward, ETriggerEvent::Triggered, this, &AFighterTank::MoveForward);
		EnhancedInputComponent->BindAction(IA_MRight, ETriggerEvent::Triggered, this, &AFighterTank::MoveRight);
		EnhancedInputComponent->BindAction(IA_TurnRight, ETriggerEvent::Triggered, this, &AFighterTank::TurnRight);
		EnhancedInputComponent->BindAction(IA_TurnCamera, ETriggerEvent::Triggered, this, &AFighterTank::TurnCamera);
		EnhancedInputComponent->BindAction(IA_ZoomCamera, ETriggerEvent::Triggered, this, &AFighterTank::ZoomCamera);
		EnhancedInputComponent->BindAction(IA_TankMode, ETriggerEvent::Triggered, this, &AFighterTank::ChangeTankMode);
	}
}

// Called to bind functionality to input
void AFighterTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// DepricatedBindAxis(PlayerInputComponent);
	BindEnhancedInputActions(PlayerInputComponent);
}

void AFighterTank::MoveForward(float val){
	if (val !=	0.0f )
	{
		this->AddMovementInput(GetActorForwardVector(), MovementSpeed*GetWorld()->GetDeltaSeconds()*val, true);
		// FloatingPawnMovement->MoveUpdatedComponent(GetActorForwardVector() * MovementSpeed*GetWorld()->GetDeltaSeconds()*val, FRotator(0.0f), true);
	}
}

void AFighterTank::MoveForward(const FInputActionValue& val)
{
	MoveForward(val.Get<float>());
}

void AFighterTank::MoveRight(float val){
	if (val !=	0.0f )
	{
		this->AddMovementInput(GetActorRightVector(), MovementSpeed*GetWorld()->GetDeltaSeconds()*val, true);
		// FloatingPawnMovement->MoveUpdatedComponent(GetActorRightVector() * MovementSpeed*GetWorld()->GetDeltaSeconds()*val, FRotator(0.0f), true);
	}
}

void AFighterTank::MoveRight(const struct FInputActionValue& val)
{
	MoveRight(val.Get<float>());
}

void AFighterTank::TurnRight(float val){
	if (val !=	0.0f )
	{
		// this->AddControllerYawInput(val);
		TankBody->SetWorldRotation(TankBody->GetComponentRotation().Add(0.0, val, 0.0));
	}
}

void AFighterTank::TurnRight(const struct FInputActionValue& val)
{
	TurnRight(val.Get<float>());
}

void AFighterTank::RotateWeapons(float YawVal, float PitchVal)
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

void AFighterTank::TurnYawView(float val){
	if (val !=	0.0f )
	{
		SpringArm->AddRelativeRotation(FRotator(0.0f, val, 0.0f));
		RotateWeapons(val, 0.0);
	}
}

void AFighterTank::TurnPitchView(float val){
	if (val !=	0.0f )
	{
		SpringArm->AddRelativeRotation(FRotator(val, 0.0f, 0.0f));
		RotateWeapons(0.0, val);
	}
}

void AFighterTank::RotateSpringArm(FVector2D MouseXY)
{
	FRotator NewRotation = SpringArm->GetRelativeRotation().Add(MouseXY.Y ,MouseXY.X,0.0f);
	if (NewRotation.Pitch < -3.0f && NewRotation.Pitch > -85.0)
		SpringArm->SetRelativeRotation(NewRotation);
	else
	{
		SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch ,TankTurret->GetRelativeRotation().Yaw,0.0f));
	}
}

void AFighterTank::RotateTurret(FVector2D MouseXY)
{
	TankTurret->AddRelativeRotation(FRotator(0.0, MouseXY.X, 0.0f));
}

void AFighterTank::RotateWeapons(FVector2D MouseXY)
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

void AFighterTank::TurnCamera(const struct FInputActionValue& val)
{
	FVector2D MouseXY = val.Get<FVector2D>();
	if (MouseXY.X != 0.0 || MouseXY.Y != 0.0 )
	{
		RotateSpringArm(MouseXY);
		RotateTurret(MouseXY);
		RotateWeapons(MouseXY);
	}
	
		
}

void AFighterTank::ZoomCamera(float val){
	float NewLength = SpringArm->TargetArmLength + ZoomSpeed*val;
	if (val !=	0.0f && NewLength <= 3000.0 && NewLength >= 300.0)
	{
		SpringArm->TargetArmLength = NewLength;
	}
}

void AFighterTank::ZoomCamera(const struct FInputActionValue& val)
{
	ZoomCamera(val.Get<float>());
}

void AFighterTank::ChangeTankMode(const struct FInputActionValue& val)
{
}
