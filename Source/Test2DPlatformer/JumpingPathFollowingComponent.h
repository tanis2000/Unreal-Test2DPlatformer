// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "JumpingPathFollowingComponent.generated.h"

class UNavMovementComponent;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API UJumpingPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

protected:
	/** cached UCharacterMovementComponent */
	UPROPERTY(transient)
	UCharacterMovementComponent* CharacterMoveComp;

	/** cached UPlatformerPawnMovementComponent */
	UPROPERTY(transient)
	class UPlatformerPawnMovementComponent *PlatformerMovementComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JumpingPathFollowingComponent")
	bool DrawDebug;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="JumpingPathFollowingComponent")
	FVector Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JumpingPathFollowingComponent")
	bool bUseNavAreas = true;
	
public:
	// used to detect properties of a path's segment a character is about to follow
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	// used to cache UCharacterMovementComponent we're using in SetMoveSegment implementation
	// virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;
	virtual void SetNavMovementInterface(INavMovementInterface* NavMoveInterface) override;
	
	virtual void FollowPathSegment(float DeltaTime) override;
};
