// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameViewportClient.h"
#include "Engine.h"
#include "PlatformerGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API UPlatformerGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed = 1.f, bool bGamepad = false) override;
	
	
};
