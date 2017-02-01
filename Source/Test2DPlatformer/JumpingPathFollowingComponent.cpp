// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "JumpingPathFollowingComponent.h"
#include "NavArea_Jump.h"

void UJumpingPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
    Super::SetMoveSegment(SegmentStartIndex);

    if (CharacterMoveComp != NULL)
    {
        const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

        if (FNavAreaHelper::HasJumpFlag(SegmentStart))
        {
            UE_LOG(LogTemp, Warning, TEXT("Has Jump flag."));
            // jump! well... fly-in-straight-line!
            CharacterMoveComp->SetMovementMode(MOVE_Flying);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Missing Jump flag."));
            // regular move
            CharacterMoveComp->SetMovementMode(MOVE_Walking);
        }
    }
}

void UJumpingPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
    Super::SetMovementComponent(MoveComp);

    CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
}




