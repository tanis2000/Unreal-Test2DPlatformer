// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerPawnMovementComponent.h"
#include "Test2DPlatformer.h"
#include "EnemyPawn.h"

bool UPlatformerPawnMovementComponent::HasValidData() const
{
    bool bIsValid = UpdatedComponent && IsValid(PawnOwner);
    return bIsValid;
}


void UPlatformerPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    const FVector InputVector = ConsumeInputVector();
    if (!HasValidData() || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Super tick may destroy/invalidate CharacterOwner or UpdatedComponent, so we need to re-check.
    if (!HasValidData())
    {
        return;
    }

    PerformMovement(DeltaTime);


#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    /*
    const bool bVisualizeMovement = CharacterMovementCVars::VisualizeMovement > 0;
    if (bVisualizeMovement)
    {
        VisualizeMovement();
    }
     */
#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

}


void UPlatformerPawnMovementComponent::PerformMovement(float DeltaSeconds)
{
    if (!HasValidData())
    {
        return;
    }

    //UE_LOG(LogTemp, Warning, TEXT("We should update the movement"));

    AEnemyPawn *pawn = Cast<AEnemyPawn>(PawnOwner);
    if (pawn != nullptr) {
        //FVector delta = FVector(1, 0, 0);
        //FRotator rotator = FRotator();
        //MoveUpdatedComponent(delta, rotator, false);
        //UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *Velocity.ToString());
        FVector inputVector = ConsumeInputVector();
        //UE_LOG(LogTemp, Warning, TEXT("Input vector: %s"), *inputVector.ToString());
        FVector amount = FVector(Velocity.X, Velocity.Y, Velocity.Z);
        amount.Normalize();
        if (MovementMode == MOVE_Flying) {
            pawn->Jump(amount * 3.0f * 50 * DeltaSeconds);
        } else {
            pawn->Move(amount * 3.0f * 50 * DeltaSeconds);
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Cannot cast the pawn to EnemyPawn."));
    }

    UpdateComponentVelocity();

}

bool UPlatformerPawnMovementComponent::MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit, ETeleportType Teleport)
{
    UE_LOG(LogTemp, Warning, TEXT("Delta: %s"), *Delta.ToString());
    return false;

}

void UPlatformerPawnMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
    MovementMode = NewMovementMode;
}


