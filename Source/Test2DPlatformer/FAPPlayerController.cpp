// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "FAPPlayerController.h"



AFAPPlayerController::AFAPPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}
