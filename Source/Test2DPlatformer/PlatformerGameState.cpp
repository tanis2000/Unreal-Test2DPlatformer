// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameState.h"

APlatformerGameState::APlatformerGameState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PlayerName = TEXT(""); // Bad idea. This should not be here and replicated to all clients.. 
}