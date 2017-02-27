// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "FAPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API AFAPPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	
	protected:
		/** Allows the PlayerController to set up custom input bindings. */
		virtual void SetupInputComponent() override;

		void MoveSide(float value);
		void MoveVertical(float value);
		void Jump(float value);
	
};
