// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetNextLocation.h"
#include "DwarfEnemyCharacter.h"
#include "AIController.h"
#include "MyAIController.h"

EBTNodeResult::Type UBTTask_GetNextLocation::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    class ADwarfEnemyCharacter* Character = Cast<ADwarfEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());

    if (Character == nullptr)
        return EBTNodeResult::Failed;

    class AMyAIController* MyController = Cast<AMyAIController>(Character->GetController());
    class ADwarfBaseCharacter* Target = Cast<ADwarfBaseCharacter>(Character->Target);


    FVector targetLocation = Target->GetActorLocation();
    FVector CharacterForward = Character->GetActorForwardVector();
    CharacterForward *= 300;
    FVector newLocation = targetLocation - CharacterForward;

    MyController->SetTargetLocation(newLocation);

    // UE_LOG(LogTemp, Warning, TEXT("Safe Location X: %f | Y: %f"), oldLocation.X, oldLocation.Y);

    return EBTNodeResult::Succeeded;
}
