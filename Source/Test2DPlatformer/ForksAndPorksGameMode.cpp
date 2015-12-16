// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "ForksAndPorksGameMode.h"
#include "FAPPlayerController.h"
#include "Player/FAPSpectatorPawn.h"


AForksAndPorksGameMode::AForksAndPorksGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // set default pawn class to our character
    DefaultPawnClass = AFAPSpectatorPawn::StaticClass();
    SpectatorClass = AFAPSpectatorPawn::StaticClass();
    PlayerControllerClass = AFAPPlayerController::StaticClass();
    
}

void AForksAndPorksGameMode::BeginPlay()
{
    if(GEngine)
    {
        UGameUserSettings* MyGameSettings = GEngine->GetGameUserSettings();
        MyGameSettings->SetScreenResolution(FIntPoint(320 * 2, 240 * 2));
        MyGameSettings->SetFullscreenMode(EWindowMode::Windowed);
        MyGameSettings->SetVSyncEnabled(true);
        MyGameSettings->ApplySettings(true);
    }
    
}