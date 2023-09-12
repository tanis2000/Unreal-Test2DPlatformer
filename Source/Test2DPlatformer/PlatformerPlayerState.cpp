// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerPlayerState.h"
#include "Test2DPlatformer.h"
#include "Net/UnrealNetwork.h"

APlatformerPlayerState::APlatformerPlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	/* AI will remain in team 0, players are updated to team 1 through the GameMode::InitNewPlayer */
	TeamNumber = 0;
	ClearData();
}

void APlatformerPlayerState::ClearData()
{
	NumKills = 0;
	NumDeaths = 0;
	SetScore(0);
	PlayerDisplayName = FText();
}

void APlatformerPlayerState::Reset()
{
	Super::Reset();

	ClearData();
}

void APlatformerPlayerState::AddKill()
{
	NumKills++;
}

void APlatformerPlayerState::AddDeath()
{
	NumDeaths++;
}

void APlatformerPlayerState::ScorePoints(int32 Points)
{
	SetScore(GetScore() + Points);

	/* Add the score to the global score count */
    /*
	ASGameState* GS = GetWorld()->GetGameState<ASGameState>();
	if (GS)
	{
		GS->AddScore(Points);
	}
    */
}


void APlatformerPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}


int32 APlatformerPlayerState::GetTeamNumber() const
{
	return TeamNumber;
}

int32 APlatformerPlayerState::GetKills() const
{
	return NumKills;
}

int32 APlatformerPlayerState::GetDeaths() const
{
	return NumDeaths;
}

FText APlatformerPlayerState::GetPlayerDisplayName() const
{
	return PlayerDisplayName;
}

void APlatformerPlayerState::SetPlayerDisplayName(const FText Name)
{
	PlayerDisplayName = Name;
}

void APlatformerPlayerState::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlatformerPlayerState, NumKills);
	DOREPLIFETIME(APlatformerPlayerState, NumDeaths);
	DOREPLIFETIME(APlatformerPlayerState, TeamNumber);
	DOREPLIFETIME(APlatformerPlayerState, PlayerDisplayName);
}


