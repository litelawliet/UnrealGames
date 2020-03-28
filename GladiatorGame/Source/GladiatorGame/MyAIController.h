// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DwarfBaseCharacter.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AMyAIController : public AAIController
{
    GENERATED_BODY()

	UPROPERTY()
    UBlackboardComponent* BlackboardComp;

	UPROPERTY()
    UBehaviorTreeComponent* BehaviorTreeComp;

public:

    explicit AMyAIController(const class FObjectInitializer& ObjectInitializer);

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

    virtual void Tick(const float DeltaTime) override;

    void SetTargetLocation(FVector targetLocation);
    void SetNewLocation(FVector NewLocation);
    void SetTargetEnemy(APawn* NewTarget);

    ADwarfBaseCharacter* GetTargetEnemy();
    FVector GetNewLocation();

private:

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName TargetEnemyKeyName;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName SafeLocationKeyName;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName TargetLocationKeyName;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName BotTypeKeyName;

    class ADwarfEnemyCharacter* EnemyCharacter;
};
