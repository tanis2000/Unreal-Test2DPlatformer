// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlatformerGameState.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API APlatformerGameState : public AGameStateBase
{
	GENERATED_BODY()

	APlatformerGameState(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;
};
