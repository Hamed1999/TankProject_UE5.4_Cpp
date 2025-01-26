// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com


#include "Widgets/EnemyHUD.h"

#include "Components/ProgressBar.h"

void UEnemyHUD::UpdateHealth(float Health)
{
	float HealthPercentage = Health / 100.0f;
	Health_ProgressBar->SetPercent(HealthPercentage);
}

float UEnemyHUD::GetHealth()
{
	float Health = Health_ProgressBar->GetPercent()*100;
	return Health;
}
