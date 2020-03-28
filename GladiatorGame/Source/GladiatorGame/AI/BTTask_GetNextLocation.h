// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNextLocation.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTTask_GetNextLocation : public UBTTaskNode
{
	GENERATED_BODY()

    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
