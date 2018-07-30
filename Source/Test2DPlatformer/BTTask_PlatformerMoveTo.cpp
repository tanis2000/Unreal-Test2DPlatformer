// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_PlatformerMoveTo.h"
#include "Test2DPlatformer.h"
#include "EnemyPawn.h"

#include "AIController.h"

/* AI Module includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

// TODO: this no longer works in UE 4.20 .. no clue why
/*
UBTTask_PlatformerMoveTo::UBTTask_PlatformerMoveTo(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
    NodeName = "Platformer Move To";
    bUseGameplayTasks = GET_AI_CONFIG_VAR(bEnableBTAITasks);
    bNotifyTick = !bUseGameplayTasks;
    bNotifyTaskFinished = true;


    // accept only vectors
    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PlatformerMoveTo, BlackboardKey));
}
*/

EBTNodeResult::Type UBTTask_PlatformerMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* controller = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (controller == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AEnemyPawn *pawn = Cast<AEnemyPawn>(controller->GetPawn());
    const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
    if (pawn && MyBlackboard) {
        const FVector TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
        //UE_LOG(LogTemp, Warning, TEXT("Target location: %s"), *TargetLocation.ToString());
        if (TargetLocation == FAISystem::InvalidLocation) {
            //UE_LOG(LogTemp, Warning, TEXT("Invalid target location"));
            return EBTNodeResult::Failed;
        }
        //FVector myLocation = pawn->GetActorLocation();
        //pawn->MoveTowards(TargetLocation, 0.4f * 50);
        pawn->TargetLocation = TargetLocation;

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}