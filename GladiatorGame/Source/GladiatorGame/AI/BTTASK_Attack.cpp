// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTASK_Attack.h"
#include "DwarfEnemyCharacter.h"
#include "AIController.h"
#include "MyAIController.h"

EBTNodeResult::Type UBTTASK_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory)
{
    class ADwarfEnemyCharacter* Character = Cast<ADwarfEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());

    if (Character == nullptr)
        return EBTNodeResult::Failed;

    class AMyAIController* MyController = Cast<AMyAIController>(Character->GetController());

    Character->Attack();

    FVector v = MyController->GetNewLocation();

    UE_LOG(LogTemp, Warning, TEXT("Safe Location X: %f | Y: %f"), v.X, v.Y);

    return EBTNodeResult::Succeeded;
}
