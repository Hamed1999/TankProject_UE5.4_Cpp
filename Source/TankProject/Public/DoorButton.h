// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorButton.generated.h"

UCLASS()
class TANKPROJECT_API ADoorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorButton(); // Constructor

	~ADoorButton(); // Deconstructor

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; // Event Begin Play

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; // Event Tick

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditAnywhere, Category="C++ Variables | Health", Meta = (AllowPrivateAccess = "false", ClampMax="100", ClampMin="0"));
		float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, Category="Components | Mesh");
		UStaticMeshComponent*  Mesh = nullptr;
	UPROPERTY(EditAnywhere, Category="Collision");
		class USphereComponent*  SphereCollision = nullptr;
	UPROPERTY(EditAnywhere, Category="Components | Door", BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"));
		 AActor*  Door = nullptr;

	FVector FirstDoorLocation;
	FVector FirstButtonLocation;

	
};
