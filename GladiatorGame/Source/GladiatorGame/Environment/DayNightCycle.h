// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DirectionalLightComponent.h"
#include "DayNightCycle.generated.h"

UCLASS()
class GLADIATORGAME_API ADayNightCycle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayNightCycle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // The new Directional Light that is going to be used for lighting
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
    UDirectionalLightComponent* Light;

    // The speed at which the day goes by
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
        float DayLightSpeed = 2;
    // How fast should the sunrise and sunset happen ? A bigger value means faster nightfall
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
        float SunRiseSetSpeed = 5;

};
