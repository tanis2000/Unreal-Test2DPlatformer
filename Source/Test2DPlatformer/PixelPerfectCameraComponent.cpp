// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelPerfectCameraComponent.h"
#include "Test2DPlatformer.h"
#include "UnrealEngine.h"

UPixelPerfectCameraComponent::UPixelPerfectCameraComponent(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
    ProjectionMode = ECameraProjectionMode::Orthographic;
    bConstrainAspectRatio = true;

    // Prevent all automatic rotation behavior on the camera, character, and camera component
    bUsePawnControlRotation = false;
}

void UPixelPerfectCameraComponent::BestFit() {
    float targetHeight = DesignHeight;
    float multiplier = MinimumMultiplier;

    // Read the resolution
    FVector2D resolution = FVector2D( 1, 1 );
    resolution.X = GSystemResolution.ResX;
    resolution.Y = GSystemResolution.ResY;

    multiplier = resolution.Y / targetHeight;
    multiplier -= FMath::Fmod(multiplier, 2);
    if(multiplier < 2){
        multiplier = MinimumMultiplier;
    }

    float aspect = DesignWidth / DesignHeight;
    float height = DesignHeight * multiplier;
    float width = height * aspect;

    SetAspectRatio(aspect);
    OrthoWidth = DesignWidth;

}

