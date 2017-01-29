// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "HeroPawn.generated.h"

UCLASS()

class TEST2DPLATFORMER_API AHeroPawn : public APawn {
    GENERATED_BODY()

    // Name of the Sprite component
    static FName SpriteComponentName;

    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

    class UPixelPerfectCameraComponent *SideViewCameraComponent;

private:
    /** The main skeletal mesh associated with this Character (optional sub-object). */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))

    class UPaperFlipbookComponent *Sprite;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent *HitBoxComponent;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent *WorldCollisionBoxComponent;

    bool IgnoreJumpThrus;

    float groundAccel = 1.00f;
    float groundFric  = 3.00f;
    float airAccel    = 0.75f;
    float airFric     = 0.53f;
    float vxMax       = 3.0f;
    float vyMax       = 7.0f;
    float jumpHeight  = 8.0f;
    float gravNorm    = -0.76f;
    float gravSlide   = -0.55f;
    bool sticking = false;
    bool canStick = false;
    bool doubleJumped = false;

    static float Approach(float start, float end, float shift);

protected:
    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)

    class UPaperFlipbook *IdleAnimation;

    FVector CurrentVelocity;
    FVector SubPixelCounter;

public:
    bool InputLeft;
    bool InputWasLeft;
    bool InputRight;
    bool InputWasRight;
    bool InputDown;
    bool InputWasDown;
    bool InputJump;
    bool InputWasJump;
    bool InputFire;
    bool InputWasFire;

    bool Grounded;
    bool BottomCollided;
    bool LeftCollided;
    bool RightCollided;
    bool TopCollided;
    bool LadderCollided;
    bool OnLeftPrev;
    bool OnRightPrev;
    bool OnGroundPrev;
    bool OnTopPrev;
    bool OnLadderPrev;

    bool Dead;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector Velocity;
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector Scale;

    int Facing = 1;

    // Sets default values for this pawn's properties
    AHeroPawn();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent *InputComponent) override;

    /** Called for side to side input */
    void MoveSide(float Value);

    void MoveVertical(float Value);

    void Jump(float Value);

    void SpawnBunnies();

    void NotifyActorBeginOverlap(AActor *OtherActor) override;

    bool MoveH(float moveH);

    bool MoveV(float moveV);

    AActor *CollideFirst(float x, float y);

};
