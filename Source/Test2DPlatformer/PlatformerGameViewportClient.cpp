// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerGameViewportClient.h"
#include "Test2DPlatformer.h"


bool UPlatformerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (IgnoreInput() || EventArgs.IsGamepad() || EventArgs.Key.IsMouseButton())
	{
		return Super::InputKey(EventArgs);
	}
	else
	{
		// Propagate keyboard events to all players
		UEngine* const Engine = GetOuterUEngine();
		int32 const NumPlayers = Engine ? Engine->GetNumGamePlayers(this) : 0;
		bool bRetVal = false;
		FInputKeyEventArgs EventArgsNext = FInputKeyEventArgs(EventArgs);
		for (int32 i = 0; i < NumPlayers; i++)
		{
			EventArgsNext.ControllerId = i;
			bRetVal = Super::InputKey(EventArgsNext) || bRetVal;
		}
 
		return bRetVal;
	}
}

