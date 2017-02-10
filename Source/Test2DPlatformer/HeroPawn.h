// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BasePawn.h"
#include "HeroPawn.generated.h"

UCLASS()

class TEST2DPLATFORMER_API AHeroPawn : public ABasePawn {
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

    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float groundAccel = 1.00f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float groundFric  = 3.00f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float airAccel    = 0.75f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float airFric     = 0.53f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float vxMax       = 3.0f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float vyMax       = 7.0f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float jumpHeight  = 8.0f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float gravNorm    = -0.76f;
    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float gravSlide   = -0.55f;
    bool sticking = false;
    bool canStick = false;
    bool doubleJumped = false;

protected:
    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)

    class UPaperFlipbook *IdleAnimation;

    FVector CurrentVelocity;

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

    UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector Velocity;
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector Scale;

    UPROPERTY(EditDefaultsOnly, Category=Pawn)
    class UParticleSystem *PuffEmitter;

    int Facing = 1;

    bool UseOwnCamera = false;

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

    void MoveLeft();

    void MoveRight();

    void StopLeft();

    void StopRight();

    bool LeftPressed;
    bool RightPressed;

};
