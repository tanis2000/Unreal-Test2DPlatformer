// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerSpectatorPawn.h"
#include "Test2DPlatformer.h"
#include "PixelPerfectCameraComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"

APlatformerSpectatorPawn::APlatformerSpectatorPawn(const FObjectInitializer &ObjectInitializer)
: Super(ObjectInitializer)
{
    GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    bAddDefaultMovementBindings = false;
    SpectatorCameraComponent = CreateDefaultSubobject<UPixelPerfectCameraComponent>(TEXT("PixelPerfectCameraComponent"));
    SpectatorCameraComponent->bConstrainAspectRatio = true;
    SpectatorCameraComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    SpectatorCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    SpectatorCameraComponent->DesignWidth = 40 * 16;
    SpectatorCameraComponent->DesignHeight = 30 * 16;
    SpectatorCameraComponent->BestFit();

}

