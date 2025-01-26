// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "DoorButton.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ADoorButton::ADoorButton()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh =   CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Button Mesh"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh->SetupAttachment(RootComponent);
	// RootComponent = Mesh;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetSphereRadius(65.0f);
	SphereCollision->SetupAttachment(Mesh);
	
}

ADoorButton::~ADoorButton()
{
	
}


// Called when the game starts or when spawned
void ADoorButton::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%s is at location = %s."), *Mesh->GetName(), *Mesh->GetComponentLocation().ToString())
	if (Door)
		FirstDoorLocation = Door->GetActorLocation();

	FirstButtonLocation = Mesh->GetComponentLocation();
}

// Called every frame
void ADoorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADoorButton::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Door)
	{
		// UE_LOG(LogTemp, Fatal, TEXT("Fatal Error..."));
		FVector NewDoorLocation{FVector(FirstDoorLocation.X, FirstDoorLocation.Y, FirstDoorLocation.Z + 800)};
		Door->SetActorLocation(NewDoorLocation);
	}

	FVector NewButtonLocation{FVector(FirstButtonLocation.X, FirstButtonLocation.Y, FirstButtonLocation.Z - 30)};
	Mesh->SetWorldLocation(NewButtonLocation);
}


void ADoorButton::NotifyActorEndOverlap(AActor* OtherActor) {
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (Door)
	{
		Door->SetActorLocation(FirstDoorLocation);
	}

	// Mesh->SetWorldLocation(FirstButtonLocation);
}
