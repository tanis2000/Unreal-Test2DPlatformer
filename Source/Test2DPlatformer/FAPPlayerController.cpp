// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "FAPPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Components/InputComponent.h"

AFAPPlayerController::AFAPPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    //bShowMouseCursor = true;
    //DefaultMouseCursor = EMouseCursor::Crosshairs;
    bAutoManageActiveCameraTarget = false;
}

void AFAPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    /*
    int32 playerId = GetLocalPlayer()->GetControllerId();
    if (playerId == 0) {
        InputComponent->BindAxis("MoveSide_P1", this, &AFAPPlayerController::MoveSide);
        InputComponent->BindAxis("MoveVertical_P1", this, &AFAPPlayerController::MoveVertical);
        InputComponent->BindAxis("Jump_P1", this, &AFAPPlayerController::Jump);
    } else {
        InputComponent->BindAxis("MoveSide_P2", this, &AFAPPlayerController::MoveSide);
        InputComponent->BindAxis("MoveVertical_P2", this, &AFAPPlayerController::MoveVertical);
        InputComponent->BindAxis("Jump_P2", this, &AFAPPlayerController::Jump);
    }
    */
}

void AFAPPlayerController::MoveSide(float value)
{    
}

void AFAPPlayerController::MoveVertical(float value)
{    
}

void AFAPPlayerController::Jump(float value)
{    
}