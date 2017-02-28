// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePawn.h"
#include "Ball.generated.h"

class AHeroPawn;

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API ABall : public ABasePawn
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
	
    // Sets default values for this pawn's properties
    ABall();

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;
	
	AHeroPawn *HeroCollides(float x, float z);

public:
    float vyMax       = 7.0f;
    float gravNorm    = -0.76f;

    bool BottomCollided;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector Velocity;

	AHeroPawn *Carrier;
	
};
