// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CampFire.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

/**
 * This is a fire that burns forever, and deals damage to whoever steps on it
 */
UCLASS()
class GLADIATORGAME_API ACampFire : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACampFire();

    // The visual representation of the fire
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Fire;

    // How much damage should the fire deal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Damage Settings")
		float FireDamageAmount;

    // How long until the fire deals damage again
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Damage Settings")
		float FireDamageCooldown;

    // The collider component for the fire
	UPROPERTY(EditAnywhere)
		UBoxComponent* MyBoxComponent;

    // The class of Damage used for the fire
	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> FireDamageType;

    // Reference to the character affected by the fire
   	UPROPERTY(EditAnywhere)
		AActor* MyCharacter;

    // Hit reference recorder by OnOverlapBegin, used to deal damage 
	UPROPERTY(EditAnywhere)
		FHitResult MyHit;

    // Should the fire apply damage or not
	bool bCanApplyDamage;
    // Timer for the Fire Damage
	FTimerHandle FireTimerHandle;

	// declare component overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare component overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Deal Damage from Fire exposure
	UFUNCTION()
		void ApplyFireDamage();
};
