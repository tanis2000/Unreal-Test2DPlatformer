// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeaderboardActor.generated.h"

USTRUCT(BlueprintType)
struct FScore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Score;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataLoaded, AActor*, InstigatorActor);

UCLASS()
class TEST2DPLATFORMER_API ALeaderboardActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALeaderboardActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString GameName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Page;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 PageSize;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ProcessLeaderboardResponse(const FString& ResponseContent);
	void ProcessMemberObject(const TSharedPtr<FJsonObject>& JsonResponseObject);
	void ProcessMembersList(const TArray<TSharedPtr<FJsonValue>>& JsonResponseArray);
	void ProcessLeaderboardObject(const TSharedPtr<FJsonValue>& JsonResponseObject);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RetrieveData();

	UFUNCTION(BlueprintCallable)
	TArray<FScore> GetScores();

	UFUNCTION(BlueprintCallable)
	void SubmitScore(FString Name, int32 Score);

private:
	TArray<FScore> Scores;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnDataLoaded OnDataLoaded;
};
