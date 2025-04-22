// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpingPathFollowingComponent.h"
#include "Test2DPlatformer.h"
#include "NavArea_Jump.h"
#include "PlatformerPawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UJumpingPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	if (!bUseNavAreas)
	{
		return;
	}

	if (CharacterMoveComp != nullptr)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

		if (FNavAreaHelper::HasJumpFlag(SegmentStart))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Has Jump flag."));
			// jump! well... fly-in-straight-line!
			CharacterMoveComp->SetMovementMode(MOVE_Flying);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Missing Jump flag."));
			// regular move
			CharacterMoveComp->SetMovementMode(MOVE_Walking);
		}
	}

	if (PlatformerMovementComponent != nullptr)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];
		if (FNavAreaHelper::IsNavLink(SegmentStart))
		{
			UE_LOG(LogTemp, Display, TEXT("Path is nav link"));
		}
		if (Path->ContainsAnyCustomLink())
		{
			UE_LOG(LogTemp, Display, TEXT("Path contains custom link"));
		}
	}
}

void UJumpingPathFollowingComponent::SetNavMovementInterface(INavMovementInterface* NavMoveInterface)
{
	Super::SetNavMovementInterface(NavMoveInterface);

	// TODO: I am not 100% sure this is the way to go now.
	CharacterMoveComp = Cast<UCharacterMovementComponent>(NavMovementInterface.Get());
	PlatformerMovementComponent = Cast<UPlatformerPawnMovementComponent>(NavMovementInterface.Get());
}

void UJumpingPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	Super::FollowPathSegment(DeltaTime);

	if (!bUseNavAreas)
	{
		return;
	}

	if (Path && DrawDebug)
	{
		// Just draw the current path
		// Path->DebugDraw(MyNavData, FColor::White, nullptr, false, 1.0f);

		// // Draw the start point of the current path segment we are traveling.
		// FNavPathPoint CurrentPathPoint{};
		// FNavigationPath::GetPathPoint(&Path->AsShared().Get(), GetCurrentPathIndex(), CurrentPathPoint);
		// DrawDebugLine(GetWorld(), CurrentPathPoint.Location, CurrentPathPoint.Location + FVector(0.f, 0.f, 200.f), FColor::Blue);
		// DrawDebugSphere(GetWorld(), CurrentPathPoint.Location + FVector(0.f, 0.f, 200.f), 25.f, 16, FColor::Blue);
		//
		// // Draw the end point of the current path segment we are traveling.
		// FNavPathPoint NextPathPoint{};
		// FNavigationPath::GetPathPoint(&Path->AsShared().Get(), GetNextPathIndex(), NextPathPoint);
		// DrawDebugLine(GetWorld(), NextPathPoint.Location, NextPathPoint.Location + FVector(0.f, 0.f, 200.f), FColor::Green);
		// DrawDebugSphere(GetWorld(), NextPathPoint.Location + FVector(0.f, 0.f, 200.f), 25.f, 16, FColor::Green);
	}

	if (Path && PlatformerMovementComponent != nullptr)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];
		if (FNavAreaHelper::HasJumpFlag(SegmentStart))
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Jump flag."));
			Target = Path->GetPathPointLocation(GetNextPathIndex()).Position;
			if (Target.Z > GetOwner()->GetActorLocation().Z)
			{
				// jump! well... fly-in-straight-line!
				UE_LOG(LogTemp, Warning, TEXT("Target is higher than us, jumping -> %s"), *Target.ToString());
				PlatformerMovementComponent->SetMovementMode(MOVE_Flying);
			}
			else
			{
				// regular move
				UE_LOG(LogTemp, Warning, TEXT("Target is lower than us, walking -> %s"), *Target.ToString());
				PlatformerMovementComponent->SetMovementMode(MOVE_Walking);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing Jump flag."));
			PlatformerMovementComponent->SetMovementMode(MOVE_Walking);
		}
	}
}
