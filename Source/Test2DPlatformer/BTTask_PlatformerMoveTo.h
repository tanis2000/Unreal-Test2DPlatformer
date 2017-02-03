// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlatformerMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API UBTTask_PlatformerMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

    /** set automatically if move should use GameplayTasks */
    uint32 bUseGameplayTasks : 1;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
