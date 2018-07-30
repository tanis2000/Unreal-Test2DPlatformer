// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float vyMax       = 7.0f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float gravNorm    = -0.76f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float idleSpeed = 6.0f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float vxMax       = 10.0f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float groundAccel = 1.00f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float groundFric  = 3.00f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float airAccel    = 0.75f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float airFric     = 0.53f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float bounceFric  = 0.8f;
    
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool BottomCollided;

    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool TopCollided;

    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool LeftCollided;

    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool RightCollided;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector Velocity;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHeroPawn *Carrier;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector targetLocation;

    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float distanceFromCarrier;

    void Shoot(float speed);
};
