// Fill out your copyright notice in the Description page of Project Settings.

#include "DayNightCycle.h"
#include <string>

// Sets default values
ADayNightCycle::ADayNightCycle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADayNightCycle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Light->AddLocalRotation({ DeltaTime * DayLightSpeed, 0, 0 });
    if (Light->GetComponentRotation().Pitch > 0)
    {
        if (Light->Intensity > 0)
        Light->SetIntensity(Light->Intensity - (DeltaTime * SunRiseSetSpeed));
    }
    else
    {
        if (Light->Intensity < 10)
            Light->SetIntensity(Light->Intensity + (DeltaTime * SunRiseSetSpeed));
        
    }
}

