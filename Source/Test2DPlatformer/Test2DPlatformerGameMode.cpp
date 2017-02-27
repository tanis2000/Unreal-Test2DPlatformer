// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "Test2DPlatformerGameMode.h"
#include "TestCharacter.h"
#include "HeroPawn.h"
#include "PlatformerSpectatorPawn.h"
#include "FAPPlayerController.h"
#include "Kismet/GameplayStatics.h"

ATest2DPlatformerGameMode::ATest2DPlatformerGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // set default pawn class to our character
    //DefaultPawnClass = ATestCharacter::StaticClass();
    DefaultPawnClass = AHeroPawn::StaticClass();
    SpectatorClass = APlatformerSpectatorPawn::StaticClass();
    PlayerControllerClass = AFAPPlayerController::StaticClass();
}

void ATest2DPlatformerGameMode::StartPlay()
{
    Super::StartPlay();

    // Spawn the second player
    UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
}
