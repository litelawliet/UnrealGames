// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorGameModeBase.h"
#include "DwarfPlayerCharacter.h"
#include "UI/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

AGladiatorGameModeBase::AGladiatorGameModeBase()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Dwarf/Player/MyDwarfPlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBar(TEXT("/Game/UI/InGame/Health_UI"));
	HUDWidgetClass = HealthBar.Class;

	// use our custom HUD class
	HUDClass = APlayerHUD::StaticClass();
	
	// add Health Bar UI to viewport
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(AActor::GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AGladiatorGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TimeBeforeReloadLevel = 0.0f;
	
	SetCurrentState(EGamePlayState::EPlaying);

	MyCharacter = Cast<ADwarfPlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemiesManager::StaticClass(), managerArray);
	if (managerArray.Num() > 0)
	{
		EnemiesManager = Cast<AEnemiesManager>(managerArray[0]);
	}
}

void AGladiatorGameModeBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyCharacter)
	{
		if (FMath::IsNearlyZero(MyCharacter->GetHealth(), 0.001f))
		{
			MyCharacter->GetCharacterMovement()->DisableMovement();
			MyCharacter->bCanBeDamaged = true;
			MyCharacter->bCanUseStamina = false;
			SetCurrentState(EGamePlayState::EGameOver);
		}
	}
	if (CurrentState == EGamePlayState::EGameOver || CurrentState == EGamePlayState::EWin)
	{
		TimeBeforeReloadLevel += DeltaTime;
	}

	if (EnemiesManager != nullptr)
	{
		if (EnemiesManager->AllEnemiesDead() && CurrentState != EGamePlayState::EGameOver)
		{
			VictoryEvent.Broadcast();
			SetCurrentState(EGamePlayState::EWin);
		}
	}

	if (TimeBeforeReloadLevel >= ReloadLevelCooldown)
		HandleNewState(CurrentState);
}

EGamePlayState AGladiatorGameModeBase::GetCurrentState() const
{
	return CurrentState;
}

void AGladiatorGameModeBase::SetCurrentState(const EGamePlayState NewState)
{
	CurrentState = NewState;
}

void AGladiatorGameModeBase::HandleNewState(const EGamePlayState NewState)
{
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
	{
		// do nothing
	}
	break;
	// Unknown/default state
	case EGamePlayState::EGameOver:
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
	case EGamePlayState::EWin:
	{
		UGameplayStatics::OpenLevel(this, FName("MainMenu"), false);
	}
	break;
	// Unknown/default state
	default:
	case EGamePlayState::EUnknown:
	{
		// do nothing
	}
	break;
	}
}
