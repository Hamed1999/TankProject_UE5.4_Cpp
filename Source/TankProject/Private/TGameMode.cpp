// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "TGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Pawns/MultiMTank.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/EnemyTank.h"
#include "Widgets/PlayerHUD.h"

#define OUT

template <class T> TSubclassOf<T> ATGameMode::FindTSubclassOfByAddress(const TCHAR* ClassPath)
{
	if (ClassPath == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Class Path is empty!"));
		return nullptr;
	}
	ConstructorHelpers::FClassFinder<T> ClassRef(ClassPath);
	if (ClassRef.Succeeded())
	{
		return ClassRef.Class;
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to load Class at path: %s"), ClassPath);
	return nullptr;
}

void ATGameMode::CountAllEnemiesInLevel()
{
	if(!EnemyTankClass) return;
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, EnemyTankClass, OUT Enemies);
	AllEnemiesNum = Enemies.Num();
	AliveEnemiesNum = AllEnemiesNum;
	UE_LOG(LogTemp, Error, TEXT("Enemies Number = %d"),AllEnemiesNum);
	UpdateEnemiesNumHUD();
}

void ATGameMode::UpdateEnemiesNumHUD()
{
	OnEnemyDies.Broadcast(AliveEnemiesNum, AllEnemiesNum);
}

void ATGameMode::SetPlayerHUDClass()
{
	PlayerHUDClass = FindTSubclassOfByAddress<UPlayerHUD>(TEXT("/Game/UI/Widgets/WBP_HUD"));
}

void ATGameMode::SetEnemyTankClass()
{
	EnemyTankClass = AEnemyTank::StaticClass(); // FindTSubclassOfByAddress<AEnemyTank>(TEXT("/Script/TankProject.EnemyTank"));
}

void ATGameMode::SetMultiMTankAsDefaultPawnClass()
{
	DefaultPawnClass = AMultiMTank::StaticClass();
}

void ATGameMode::SetGameOverClass()
{
	GameOverClass = FindTSubclassOfByAddress<UUserWidget>(TEXT("/Game/UI/Widgets/WBP_GameOver"));
}

void ATGameMode::SetLoadingClass()
{
	LoadingClass = FindTSubclassOfByAddress<UUserWidget>(TEXT("/Game/UI/Widgets/WBP_Loading"));
}

ATGameMode::ATGameMode()
{
	SetMultiMTankAsDefaultPawnClass();
	SetEnemyTankClass();
	SetPlayerHUDClass();
	SetGameOverClass();
	SetLoadingClass();
}

void ATGameMode::AddPlayerHUDToViewPort()
{
	WBP_HUD = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDClass, FName("Player HUD"));
	WBP_HUD->AddToViewport();
}

void ATGameMode::EnableInputs()
{
	APawn* FirstPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FirstPawn->EnableInput(FirstPawn->GetLocalViewingPlayerController());
}

void ATGameMode::SetInputModeGameOnly()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
}

void ATGameMode::AddLoadingToViewport()
{
	if(!LoadingClass) return;
	WBP_Loading = CreateWidget<UUserWidget>(GetWorld(), LoadingClass, FName("Loading Widget"));
	if(!WBP_Loading) return;
	WBP_Loading->AddToViewport(); 
}

void ATGameMode::RemoveLoadingWBP()
{
	EnableInputs();
	if (!WBP_Loading) return;
		WBP_Loading->RemoveFromParent();
}

void ATGameMode::Loading()
{
	DisableInputs();
	AddLoadingToViewport();
	FTimerHandle LoadingTimerHandle;
	GetWorldTimerManager().SetTimer(LoadingTimerHandle, this, &ATGameMode::RemoveLoadingWBP, 3.f);
	FTimerHandle CountTimerHandle;
	GetWorldTimerManager().SetTimer(CountTimerHandle, this, &ATGameMode::CountAllEnemiesInLevel, 1.f);
}

void ATGameMode::StartNewLevel()
{
	AddPlayerHUDToViewPort();
	Loading();
	SetInputModeGameOnly();
}

void ATGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartNewLevel();
}

void ATGameMode::DecreaseAlliveEnemiesNum()
{
	AliveEnemiesNum--;
	UpdateEnemiesNumHUD();
	if (AliveEnemiesNum == 0)
		GameOver();
}

void ATGameMode::DisableInputs()
{
	APawn* FirstPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FirstPawn->DisableInput(FirstPawn->GetLocalViewingPlayerController());
}

void ATGameMode::AddGameOverToViewport()
{
	WBP_GameOver = CreateWidget<UUserWidget>(GetWorld(), GameOverClass, FName("GameOver Widget"));
	WBP_GameOver->AddToViewport();
}

void ATGameMode::SetInputModeUIOnly()
{
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//PlayerController->SetInputMode(FInputModeUIOnly());
	WBP_GameOver->SetIsFocusable(true);
	FInputModeUIOnly InputModeUIOnly = FInputModeUIOnly();
	TSharedPtr<SWidget> WidgetToFocus = WBP_GameOver->TakeWidget();
	InputModeUIOnly.SetWidgetToFocus(WidgetToFocus);
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeUIOnly);
	//WBP_GameOver->SetFocus(); // keyboard inputs
}

void ATGameMode::RemoveHUD()
{
	if (!WBP_HUD) return;
		WBP_HUD->RemoveFromParent();
}

void ATGameMode::GameOver()
{
	DisableInputs();
	RemoveHUD();
	AddGameOverToViewport();
	SetInputModeUIOnly();
}