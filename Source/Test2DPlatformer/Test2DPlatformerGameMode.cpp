// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformerGameMode.h"
#include "Test2DPlatformer.h"
#include "TestCharacter.h"
#include "HeroPawn.h"
#include "PlatformerSpectatorPawn.h"
#include "FAPPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PlatformerPlayerState.h"

ATest2DPlatformerGameMode::ATest2DPlatformerGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // set default pawn class to our character
    //DefaultPawnClass = ATestCharacter::StaticClass();
    DefaultPawnClass = AHeroPawn::StaticClass();
    SpectatorClass = APlatformerSpectatorPawn::StaticClass();
    PlayerControllerClass = AFAPPlayerController::StaticClass();
	PlayerStateClass = APlatformerPlayerState::StaticClass();

	PlayerTeamNum = 0;
}

void ATest2DPlatformerGameMode::StartPlay()
{
    Super::StartPlay();

    // Spawn the second player
    // NOTE: uncomment to enable again the second player
    UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
}

AActor* ATest2DPlatformerGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerStart* BestStart = nullptr;
	AFAPPlayerController *pc = Cast<AFAPPlayerController>(Player);
	if (pc != nullptr) {
		APlatformerPlayerState *playerState = Cast<APlatformerPlayerState>(pc->PlayerState);
		if (playerState != nullptr) {
			int32 playerId = playerState->GetTeamNumber();
			FString playerNumString = FString::Printf(TEXT("%d"), playerId);
			UE_LOG(LogTemp, Warning, TEXT("Looking for a player start for %s"), *playerNumString);
			FName playerNum = FName(*playerNumString);

			/* Get all playerstart objects in level */
			TArray<AActor*> PlayerStarts;
			UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

			for (int32 i = 0; i < PlayerStarts.Num(); i++)
			{
				APlayerStart* TestStart = Cast<APlayerStart>(PlayerStarts[i]);
				if (TestStart && TestStart->PlayerStartTag.Compare(playerNum) == 0) {
					BestStart = TestStart;
					UE_LOG(LogTemp, Warning, TEXT("Found the player start: %s"), *TestStart->PlayerStartTag.ToString());
				}
			}
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Cannot find player state. State is: %s"), *AActor::GetDebugName(pc->PlayerState));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Cannot cast to AFAPPlayerController. Controller is: %s"), *AActor::GetDebugName(Player));
	}

	/* If we failed to find any (so BestStart is nullptr) fall back to the base code */
	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

FString ATest2DPlatformerGameMode::InitNewPlayer(class APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	APlatformerPlayerState* NewPlayerState = Cast<APlatformerPlayerState>(NewPlayerController->PlayerState);
	if (NewPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting Team Number to %d"), PlayerTeamNum);
		NewPlayerState->SetTeamNumber(PlayerTeamNum);
		// Make sure the next player will be in a new team
		PlayerTeamNum++;
	}

	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	return Result;
}
