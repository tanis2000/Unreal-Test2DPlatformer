// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "PopText.generated.h"

UCLASS()
class TEST2DPLATFORMER_API APopText : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APopText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBillboardComponent> Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UUserWidget> CurrentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZFinalOffset;

	float CurrentDuration;
	float CurrentZOffset;
protected:


	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdatePosition(float DeltaTime);
};
