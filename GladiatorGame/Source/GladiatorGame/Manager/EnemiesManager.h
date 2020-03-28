// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DwarfBaseCharacter.h"
#include "EnemiesManager.generated.h"

UCLASS()
class GLADIATORGAME_API AEnemiesManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemiesManager();
	bool AllEnemiesDead() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ADwarfBaseCharacter> DwarfSubClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> Spawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEnemiesDead = false;

private:
	int NbDeadEnemies = 0;
};
