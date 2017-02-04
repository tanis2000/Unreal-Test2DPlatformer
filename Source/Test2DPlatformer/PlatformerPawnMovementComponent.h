// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PlatformerPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API UPlatformerPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(Category="Character Movement: MovementMode", BlueprintReadOnly)
    TEnumAsByte<enum EMovementMode> MovementMode;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    bool HasValidData() const;

    void PerformMovement(float DeltaSeconds);

    virtual bool MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit = NULL, ETeleportType Teleport = ETeleportType::None) override;

    virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0);

};
