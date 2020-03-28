#pragma once

#include "GameFramework/GameModeBase.h"
#include "DwarfEnemyCharacter.h"
#include "Manager/EnemiesManager.h"
#include "GladiatorGameModeBase.generated.h"

class ADwarfPlayerCharacter;

UENUM()
enum class EGamePlayState : uint8
{
	EPlaying,
	EGameOver,
	EWin,
	EUnknown
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictoryDelegate);

/**
 * @brief The Gamemode we will use on the Arena Map
 */
UCLASS()
class GLADIATORGAME_API AGladiatorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGladiatorGameModeBase();

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(const float DeltaTime) override;

    // Reference to playerCharacter
	ADwarfPlayerCharacter* MyCharacter;

	// Returns the current playing state 
	UFUNCTION(BlueprintPure, Category = "Health")
		EGamePlayState GetCurrentState() const;

	// Sets a new playing state 
	void SetCurrentState(const EGamePlayState NewState);

    // The widget type for the health widget
	UPROPERTY(EditAnywhere, Category = "Health")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

    // The current widget in use
	UPROPERTY(EditAnywhere, Category = "Health")
		class UUserWidget* CurrentWidget;

	
	// Keeps track of the current playing state 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Settings")
		EGamePlayState CurrentState;

    // Delay for the level reload
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Settings")
		float ReloadLevelCooldown = 5.0f;

    // The Event for when Victory occurs
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnVictoryDelegate VictoryEvent;

    // This script handles the enemies throughout the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AEnemiesManager* EnemiesManager;
	
private:
    // This is a counter for how much time has passed since the VictoryEvent
	float TimeBeforeReloadLevel;

	// Handle any function calls that rely on the player-state of our game
	void HandleNewState(const EGamePlayState NewState);
	UPROPERTY()
	TArray<AActor*> managerArray;
};
