// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "PlatformerSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API APlatformerSpectatorPawn : public ASpectatorPawn
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

	class UPixelPerfectCameraComponent *SpectatorCameraComponent;

	
	
};
