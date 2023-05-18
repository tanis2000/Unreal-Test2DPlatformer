// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerCameraActor.h"
#include "Test2DPlatformer.h"
#include "PixelPerfectCameraComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
APlatformerCameraActor::APlatformerCameraActor(): AActor()
{
	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SpectatorCameraComponent = CreateDefaultSubobject<
		UPixelPerfectCameraComponent>(TEXT("PixelPerfectCameraComponent"));
	SpectatorCameraComponent->bConstrainAspectRatio = true;
	SpectatorCameraComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SpectatorCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SpectatorCameraComponent->DesignWidth = 40 * 16;
	SpectatorCameraComponent->DesignHeight = 30 * 16;
	SpectatorCameraComponent->BestFit();
	SpectatorCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlatformerCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformerCameraActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

