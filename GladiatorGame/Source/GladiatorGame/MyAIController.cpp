// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "DwarfEnemyCharacter.h"

AMyAIController::AMyAIController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    BehaviorTreeComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
    BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
    TargetEnemyKeyName = "TargetPlayer";
    SafeLocationKeyName = "SafeLocation";
    TargetLocationKeyName = "TargetLocation";
}

void AMyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    EnemyCharacter = Cast<ADwarfEnemyCharacter>(InPawn);

    if (EnemyCharacter)
    {
        if (EnemyCharacter->EnemyBT->BlackboardAsset)
        {
            BlackboardComp->InitializeBlackboard(*EnemyCharacter->EnemyBT->BlackboardAsset);

            BlackboardComp->SetValueAsEnum(BotTypeKeyName, EnemyCharacter->BotType);
        }

        BehaviorTreeComp->StartTree(*EnemyCharacter->EnemyBT);
    }
}

void AMyAIController::OnUnPossess()
{
    Super::OnUnPossess();
    BehaviorTreeComp->StopTree();
}

void AMyAIController::Tick(const float DeltaTime)
{
    float angle = EnemyCharacter->GetActorRotation().Yaw;

    FVector target = EnemyCharacter->GetActorLocation();

    float X = 100 * cosf(angle);
    float Y = 100 * sinf(angle);

    X = target.X - X;
    Y = target.Y - Y;

    FVector newLocation = FVector(X, Y, EnemyCharacter->GetActorLocation().Z);

    // SetNewLocation(newLocation);

    if (EnemyCharacter->HealthPercentage <= 0)
    {
        OnUnPossess();
    }
}

void AMyAIController::SetTargetLocation(FVector targetLocation)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(TargetLocationKeyName, targetLocation);
    }
}

void AMyAIController::SetNewLocation(FVector NewLocation)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(SafeLocationKeyName, NewLocation);
    }
}

void AMyAIController::SetTargetEnemy(APawn* NewTarget)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
    }
}

ADwarfBaseCharacter* AMyAIController::GetTargetEnemy()
{
    if (BlackboardComp)
    {
        return Cast<ADwarfBaseCharacter>(BlackboardComp->GetValueAsObject(TargetEnemyKeyName));
    }

    return nullptr;
}

FVector AMyAIController::GetNewLocation()
{
    return BlackboardComp->GetValueAsVector(SafeLocationKeyName);
}
