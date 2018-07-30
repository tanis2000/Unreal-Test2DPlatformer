// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "PixelPerfectCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API UPixelPerfectCameraComponent : public UCameraComponent
{
    GENERATED_UCLASS_BODY()
	
	
	
public:
    float DesignWidth = 320;
    float DesignHeight = 240;
    float MinimumMultiplier = 1;

    void BestFit();
};
