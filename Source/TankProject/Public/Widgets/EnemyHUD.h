// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHUD.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API UEnemyHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void UpdateHealth(float Health);
	UFUNCTION(BlueprintCallable)
		float GetHealth();
private:
	/**
	* Properties
	*/
	UPROPERTY(Meta=(BindWidget));
		class UProgressBar* Health_ProgressBar = nullptr;	
};
