// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlatformerCameraActor.generated.h"

UCLASS()
class TEST2DPLATFORMER_API APlatformerCameraActor : public AActor
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

	class UPixelPerfectCameraComponent *SpectatorCameraComponent;

public:
	// Sets default values for this actor's properties
	APlatformerCameraActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	
};
