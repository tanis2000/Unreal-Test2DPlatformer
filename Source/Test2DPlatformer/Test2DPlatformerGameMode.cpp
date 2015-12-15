// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "Test2DPlatformerGameMode.h"
#include "TestCharacter.h"
#include "HeroPawn.h"

ATest2DPlatformerGameMode::ATest2DPlatformerGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // set default pawn class to our character
    //DefaultPawnClass = ATestCharacter::StaticClass();
    DefaultPawnClass = AHeroPawn::StaticClass();
}


