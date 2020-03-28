// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Medkit.generated.h"

/**
 * @brief The Medkit is a power-up that can be picked from the floor
 */
UCLASS()
class GLADIATORGAME_API AMedkit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMedkit();

    // Implements Unreal's Overlap function
	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

    // This determines how much health should a medikit restore
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medikit Settings")
		float HealingValue;
};
