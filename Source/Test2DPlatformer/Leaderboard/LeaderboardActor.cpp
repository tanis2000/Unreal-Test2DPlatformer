// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderboardActor.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"


// Sets default values
ALeaderboardActor::ALeaderboardActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameName = TEXT("temp");
	Page = 1;
	PageSize = 5;
}

// Called when the game starts or when spawned
void ALeaderboardActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeaderboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALeaderboardActor::RetrieveData()
{
	UE_LOG(LogTemp, Display, TEXT("Retrieving data."));
	const FString Url = "https://podium.altralogica.it/l/" + GameName + "/top/" + FString::FromInt(Page) + "?pageSize=" + FString::FromInt(PageSize);
	FHttpModule& HttpModule = FHttpModule::Get();
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> LeaderBoardRequest = HttpModule.CreateRequest();
	LeaderBoardRequest->SetVerb("GET");
	LeaderBoardRequest->SetURL(Url);
	LeaderBoardRequest->OnProcessRequestComplete().BindLambda(
	[&](FHttpRequestPtr pRequest,
		FHttpResponsePtr pResponse,
		bool connectedSuccessfully) mutable
	{
		if (connectedSuccessfully)
		{
			ProcessLeaderboardResponse(pResponse.Get()->GetContentAsString());
		} else
		{
			switch (pRequest->GetStatus()) {
			case EHttpRequestStatus::Failed_ConnectionError:
				UE_LOG(LogTemp, Error, TEXT("Connection failed."));
			default:
				UE_LOG(LogTemp, Error, TEXT("Request failed."));
			}
		}
	});
	LeaderBoardRequest->ProcessRequest();
}

void ALeaderboardActor::ProcessLeaderboardResponse(const FString& ResponseContent)
{
	// Validate http called us back on the Game Thread...
	check(IsInGameThread());

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);
	TSharedPtr<FJsonValue> OutObject;

	if (FJsonSerializer::Deserialize(JsonReader, OutObject))
	{
		ProcessLeaderboardObject(OutObject);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot deserialize response."));
	}
}

void ALeaderboardActor::ProcessLeaderboardObject(const TSharedPtr<FJsonValue>& JsonResponseValue)
{
	if (JsonResponseValue)
	{
		const TArray<TSharedPtr<FJsonValue>> *OutArray;
		if (JsonResponseValue.Get()->AsObject()->TryGetArrayField("members", OutArray))
		{
			ProcessMembersList(*OutArray);
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("No objects in list"));
	}
}

void ALeaderboardActor::ProcessMembersList(const TArray<TSharedPtr<FJsonValue>>& JsonResponseArray)
{
	Scores.Empty();
	UE_LOG(LogTemp, Display, TEXT("Contains %d objects"), JsonResponseArray.Num());
	for (int i = 0; i < JsonResponseArray.Num(); ++i)
	{
		ProcessMemberObject(JsonResponseArray[i]->AsObject());
	}
	OnDataLoaded.Broadcast(this);
}

void ALeaderboardActor::ProcessMemberObject(const TSharedPtr<FJsonObject>& JsonResponseObject)
{
	if (JsonResponseObject)
	{
		FString name = TEXT("");
		if (JsonResponseObject->TryGetStringField("publicID", name))
		{
			UE_LOG(LogTemp, Display, TEXT("Added %s"), *FString(name));
		}
		
		int32 Score = 0;
		if (JsonResponseObject->TryGetNumberField("score", Score))
		{
			UE_LOG(LogTemp, Display, TEXT("Added %d"), Score);
		}
		FScore Sc = { name, Score };
		Scores.Add(Sc);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Error processing member object"));
	}
}

TArray<FScore> ALeaderboardActor::GetScores()
{
	return Scores;
}

void ALeaderboardActor::SubmitScore(FString Name, int32 Score)
{
	UE_LOG(LogTemp, Display, TEXT("Submitting score"));
	const FString Url = "https://podium.altralogica.it/l/" + GameName + "/members/" + Name + "/score";
	FHttpModule& HttpModule = FHttpModule::Get();
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> LeaderBoardRequest = HttpModule.CreateRequest();
	LeaderBoardRequest->SetVerb("PUT");
	LeaderBoardRequest->SetURL(Url);
	LeaderBoardRequest->SetContentAsString("{\"score\":" + FString::FromInt(Score) + "}");
	LeaderBoardRequest->OnProcessRequestComplete().BindLambda(
	[&](FHttpRequestPtr pRequest,
		FHttpResponsePtr pResponse,
		bool connectedSuccessfully) mutable
	{
		if (connectedSuccessfully)
		{
			UE_LOG(LogTemp, Error, TEXT("Score submitted."));
		} else
		{
			switch (pRequest->GetStatus()) {
			case EHttpRequestStatus::Failed_ConnectionError:
				UE_LOG(LogTemp, Error, TEXT("Connection failed."));
			default:
				UE_LOG(LogTemp, Error, TEXT("Request failed."));
			}
		}
	});
	LeaderBoardRequest->ProcessRequest();
}