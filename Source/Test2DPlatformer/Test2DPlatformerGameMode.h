// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Engine.h"
#include "Test2DPlatformerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API ATest2DPlatformerGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/* The teamnumber assigned to Players */
	int32 PlayerTeamNum;

    ATest2DPlatformerGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void StartPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	/* Called once on every new player that enters the gamemode */
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
	
};
