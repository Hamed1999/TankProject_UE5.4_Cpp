// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	
protected:
	/**
	 * Methods
	 */
	UFUNCTION()
		void OnResumeButtonClicked();
	UFUNCTION(BlueprintCallable)
		void ExitTankGame();
	/**
	* Properties
	*/
	UPROPERTY(Meta=(BindWidget));
		class UButton* ResumeButton = nullptr;
	UPROPERTY(Meta=(BindWidget));
		 UButton* OptionButton = nullptr;
	UPROPERTY(Meta=(BindWidget));
		UButton* QuitButton = nullptr;
};
