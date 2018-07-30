// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TilemapNavComponent.generated.h"

class UPaperTileMapComponent;

struct WayPoint {
    FVector Location;

    inline bool operator==(const WayPoint& Other) const
    {
        return (Other.Location.X == Location.X) && (Other.Location.Z == Location.Z);
    }

    inline bool operator!=(const WayPoint& Other) const
    {
        return (Other.Location.X != Location.X) && (Other.Location.Z != Location.Z);
    }
};

struct Link {
    WayPoint *Start;
    WayPoint *End;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST2DPLATFORMER_API UTilemapNavComponent : public UActorComponent
{
	GENERATED_BODY()
private:
    UPaperTileMapComponent* TileMapComponent;
    int32 MapWidth;
    int32 MapHeight;
    int32 NumLayers;
    TArray<WayPoint> WayPoints;
    TArray<Link> Links;

public:

    uint32 NavLayer;

	// Sets default values for this component's properties
	UTilemapNavComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
