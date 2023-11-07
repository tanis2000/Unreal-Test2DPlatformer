// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerNavLinkProxy.h"

#include "NavLinkCustomComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Test2DPlatformer/EnemyPawn.h"
#include "Test2DPlatformer/NavArea_Jump.h"


// Sets default values
APlatformerNavLinkProxy::APlatformerNavLinkProxy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlatformerNavLinkProxy::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &APlatformerNavLinkProxy::SmartLinkReached);
}

// Called every frame
void APlatformerNavLinkProxy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformerNavLinkProxy::SmartLinkReached(AActor *Actor, const FVector &DestinationLocation)
{
	UE_LOG(LogTemp, Display, TEXT("Smart link reached"));
	FVector LaunchVelocity;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), LaunchVelocity, Actor->GetActorLocation(), DestinationLocation);
	UE_LOG(LogTemp, Display, TEXT("Suggested velocity: %s"), *LaunchVelocity.ToString());
	LaunchVelocity.Normalize();
	AEnemyPawn *Pawn = Cast<AEnemyPawn>(Actor);
	if (Pawn != nullptr)
	{
		// Pawn->Jump(LaunchVelocity * Pawn->jumpHeight);
	}
}