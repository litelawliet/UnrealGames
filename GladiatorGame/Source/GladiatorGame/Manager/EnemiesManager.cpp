// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemiesManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AEnemiesManager::AEnemiesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AEnemiesManager::AllEnemiesDead() const
{
	return bEnemiesDead;
}

// Called when the game starts or when spawned
void AEnemiesManager::BeginPlay()
{
	Super::BeginPlay();
	
	bEnemiesDead = false;

	if (Spawns.Num() > 0)
	{
		for (auto It = Spawns.begin(); It != Spawns.end(); ++It)
		{
			const FVector ActorLocation = (*It)->GetActorLocation();
			Enemies.Add(GetWorld()->SpawnActor(DwarfSubClass, &ActorLocation));
		}
	}
}

// Called every frame
void AEnemiesManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	NbDeadEnemies = 0;
	
	for (auto It = Enemies.begin(); It != Enemies.end(); ++It)
	{
		class ADwarfBaseCharacter* Dwarf = Cast<ADwarfBaseCharacter>(*It);
		if (Dwarf != nullptr)
		{
			if (Dwarf->GetHealth() <= 0)
			{
				++NbDeadEnemies;
			}
		}
	}

	if (NbDeadEnemies == Enemies.Num())
	{
		bEnemiesDead = true;
	}
}

