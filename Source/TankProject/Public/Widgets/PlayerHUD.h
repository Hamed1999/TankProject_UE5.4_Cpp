// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUD(const FObjectInitializer& ObjectInitializer);
	void BindUpdateHealthProgressBar();
	void BindUpdateEnemiesNumber();
	/**
	 * Same as BeginPlay()
	 */
	virtual bool Initialize() override;
	
	
protected:
	/**
	 * Methods
	 */
	UFUNCTION()
		void OnPauseButtonClicked();
	void SetPauseMenuClass();
	UFUNCTION(BlueprintCallable)
		void UpdateHealthBar(float Health);
	UFUNCTION(BlueprintCallable)
		void UpdateEnemiesNum(int AliveEnemiesNum, int AllEnemiesNum);
	/**
	 * Properties
	 */
	UPROPERTY(Meta=(BindWidget));
		class UButton* PauseButton = nullptr;
	UPROPERTY(Meta=(BindWidget));
		class UTextBlock* Enemies = nullptr;
	UPROPERTY(Meta=(BindWidget));
		class UProgressBar* Health_Progress = nullptr;

	TSubclassOf<class UPauseMenu> PauseMenuClass = nullptr;
};
