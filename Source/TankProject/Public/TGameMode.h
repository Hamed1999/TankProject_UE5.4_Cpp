// Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDiesSignature, int, AliveEnemiesNum, int, AllEnemiesNum);

UCLASS()
class TANKPROJECT_API ATGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATGameMode();
	virtual void BeginPlay() override;
	FOnEnemyDiesSignature OnEnemyDies;
	void DecreaseAlliveEnemiesNum();
	void GameOver();
private:
	/**
	 *  Methods
	 */
	template <class T> TSubclassOf<T> FindTSubclassOfByAddress (const TCHAR* ClassPath);
	void SetEnemyTankClass();
	void SetMultiMTankAsDefaultPawnClass();
	void SetGameOverClass();
	void SetLoadingClass();
	void SetPlayerHUDClass();
	void AddPlayerHUDToViewPort();
	void SetInputModeUIOnly();
	void AddGameOverToViewport();
	void RemoveHUD();
	void EnableInputs();
	void DisableInputs();
	void SetInputModeGameOnly();
	void AddLoadingToViewport();
	void RemoveLoadingWBP();
	void Loading();
	void StartNewLevel();
	void CountAllEnemiesInLevel();
	void UpdateEnemiesNumHUD();
	/**
	* Global Properties 
	*/
	TSubclassOf<class AEnemyTank> EnemyTankClass;
	int AllEnemiesNum;
	int AliveEnemiesNum;
	TSubclassOf<class UPlayerHUD> PlayerHUDClass = nullptr;
	UPlayerHUD* WBP_HUD = nullptr;
	TSubclassOf<UUserWidget> GameOverClass = nullptr;
	TSubclassOf<UUserWidget> LoadingClass = nullptr;
	UUserWidget* WBP_Loading = nullptr;
	UUserWidget* WBP_GameOver = nullptr;
};