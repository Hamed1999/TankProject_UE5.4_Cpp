// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Widgets/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UPauseMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::ExitTankGame);
	return bSuccess;
}

void UPauseMenu::OnResumeButtonClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(),false);
	RemoveFromParent();
}

void UPauseMenu::ExitTankGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Exit"); // Quit
}
