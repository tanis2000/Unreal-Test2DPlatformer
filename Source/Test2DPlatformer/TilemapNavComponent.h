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
	UPROPERTY()
    UPaperTileMapComponent* TileMapComponent = nullptr;
	
    int32 MapWidth = 0;
    int32 MapHeight = 0;
    int32 NumLayers = 0;
    TArray<WayPoint> WayPoints;
    TArray<Link> Links;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TilemapNavComponent")
    int32 NavLayer = 0; // By default we use the first layer

	UPROPERTY(EditAnywhere, Category="TilemapNavComponent")
	TSubclassOf<AActor> WayPointClassMarker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TilemapNavComponent")
	bool bEnableLinkGeneration = true;
	
	// Sets default values for this component's properties
	UTilemapNavComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
