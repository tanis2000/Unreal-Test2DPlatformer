// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "FAPSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API AFAPSpectatorPawn : public ASpectatorPawn
{
	GENERATED_UCLASS_BODY()
	
private:
    UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UFAPCameraComponent* FAPCameraComponent;

public:
	
    /* Returns a pointer to the strategy camera component the pawn has. */
    UFAPCameraComponent* GetFAPCameraComponent();

	
};
