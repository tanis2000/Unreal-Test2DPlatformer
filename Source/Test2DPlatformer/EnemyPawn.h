// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePawn.h"
#include "EnemyPawn.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API AEnemyPawn : public ABasePawn
{
	GENERATED_BODY()

	// Name of the Sprite component
	static FName SpriteComponentName;

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent *Sprite;

protected:
    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    class UPaperFlipbook *IdleAnimation;

    UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPlatformerPawnMovementComponent *MovementComponent;


public:
    float vyMax       = 7.0f;
    float jumpHeight  = 8.0f;
    float gravNorm    = -0.76f;

    bool BottomCollided;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector Velocity;

    FVector TargetLocation;

    // Sets default values for this pawn's properties
    AEnemyPawn();

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    void Jump(FVector amount);


};
