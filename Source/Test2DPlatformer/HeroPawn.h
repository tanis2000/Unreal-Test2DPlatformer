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

protected:
    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)

    class UPaperFlipbook *IdleAnimation;

    FVector CurrentVelocity;
    FVector SubPixelCounter;

public:
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

    void SpawnBunnies();

    void NotifyActorBeginOverlap(AActor *OtherActor) override;

    bool MoveH(float moveH);

    bool MoveV(float moveV);

    AActor *CollideFirst(float x, float y);

};
