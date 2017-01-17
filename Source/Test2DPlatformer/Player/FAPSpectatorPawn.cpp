// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "FAPSpectatorPawn.h"
#include "FAPCameraComponent.h"


AFAPSpectatorPawn::AFAPSpectatorPawn(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer/*.SetDefaultSubobjectClass<UStrategySpectatorPawnMovement>(Super::MovementComponentName)*/)
{
    GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    bAddDefaultMovementBindings = false;

    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    FAPCameraComponent = CreateDefaultSubobject<UFAPCameraComponent>(TEXT("FAPCameraComponent"));
    FAPCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    FAPCameraComponent->OrthoWidth = 160 * 2;
    FAPCameraComponent->AspectRatio = 320.0f/240.0f;
    FAPCameraComponent->bConstrainAspectRatio = true;
    FAPCameraComponent->AttachParent = RootComponent;
}

UFAPCameraComponent* AFAPSpectatorPawn::GetFAPCameraComponent()
{
    check( FAPCameraComponent != NULL );
    return FAPCameraComponent;
}

