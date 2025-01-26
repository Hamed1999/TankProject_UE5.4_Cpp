// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Widgets/PlayerHUD.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/PauseMenu.h"
#include "Pawns/MultiMTank.h"
#include "TGameMode.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "EnemiesNumberNamespace"

void UPlayerHUD::SetPauseMenuClass()
{
	ConstructorHelpers::FClassFinder<UPauseMenu> PauseMenuReference(TEXT("/Game/UI/Widgets/WBP_PauseMenu"));
	if (PauseMenuReference.Succeeded())
	{
		PauseMenuClass = PauseMenuReference.Class;
		UE_LOG(LogTemp, Log, TEXT("WBP_PauseMenu reference was found."));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("WBP_PauseMenu was Not found!"));
}

void UPlayerHUD::UpdateHealthBar(float Health)
{
	float HealthPercentage = 1 - Health / 100;
	Health_Progress->SetPercent(HealthPercentage);
}

void UPlayerHUD::UpdateEnemiesNum(int AliveEnemiesNum, int AllEnemiesNum)
{
	FText EnemiesNumText = FText::Format(LOCTEXT("Enemies Number Text","{0}/{1}"), AliveEnemiesNum, AllEnemiesNum);
	Enemies->SetText(EnemiesNumText);
}

UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetPauseMenuClass();
}

void UPlayerHUD::BindUpdateHealthProgressBar()
{
	if (AMultiMTank* MultiMTank = Cast<AMultiMTank>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		MultiMTank->OnHealthChanged.AddDynamic(this, &UPlayerHUD::UpdateHealthBar);
	}
}

void UPlayerHUD::BindUpdateEnemiesNumber()
{
	if (ATGameMode* TGameMode = Cast<ATGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		TGameMode->OnEnemyDies.AddDynamic(this, &UPlayerHUD::UpdateEnemiesNum);
	}
}

bool UPlayerHUD::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	PauseButton->OnClicked.AddDynamic(this, &UPlayerHUD::OnPauseButtonClicked);
	BindUpdateHealthProgressBar();
	BindUpdateEnemiesNumber();
	
	return bSuccess;
}

void UPlayerHUD::OnPauseButtonClicked()
{
	if (!PauseMenuClass) return;
	UPauseMenu* WBP_PauseMenu = CreateWidget<UPauseMenu>(this, PauseMenuClass, FName("Pause Menu Widget"));
	WBP_PauseMenu->AddToViewport();
	UGameplayStatics::SetGamePaused(GetWorld(),true);
}