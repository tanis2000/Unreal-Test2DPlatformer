// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "PaperTileMapComponent.h"
#include "TilemapNavComponent.h"
#include "AI/Navigation/NavLinkProxy.h"
#include "AI/Navigation/NavLinkCustomComponent.h"
#include "NavArea_Jump.h"


// Sets default values for this component's properties
UTilemapNavComponent::UTilemapNavComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    NavLayer = 0; // force to use the first layer for the time being

	// ...
}


// Called when the game starts
void UTilemapNavComponent::BeginPlay()
{
	Super::BeginPlay();

    AActor *actor = GetOwner();
    if (actor != nullptr) {
        TileMapComponent = actor->FindComponentByClass<UPaperTileMapComponent>();
        if (TileMapComponent != nullptr) {
            TileMapComponent->GetMapSize(MapWidth, MapHeight, NumLayers);
            UE_LOG(LogTemp, Warning, TEXT("Map size, X: %d, Y: %d Layers: %d"), MapWidth, MapHeight, NumLayers);

            // Build the connection between jumping/fallout positions
            for (uint32 y = 0 ; y < MapHeight; y++) {
                for (uint32 x = 0 ; x < MapWidth; x++) {
                    FPaperTileInfo tile = TileMapComponent->GetTile(x, y, NavLayer);
                    int32 tileIndex = tile.GetTileIndex();
                    int32 packedTileIndex = tile.PackedTileIndex;
                    if (packedTileIndex != INDEX_NONE) {
                        UE_LOG(LogTemp, Warning, TEXT("X: %d, Y: %d, Tile: %d"), x, y, tile.GetTileIndex());

                        FPaperTileInfo upTile = TileMapComponent->GetTile(x, y-1, NavLayer);
                        FPaperTileInfo upLeftTile = TileMapComponent->GetTile(x-1, y-1, NavLayer);
                        FPaperTileInfo upRightTile = TileMapComponent->GetTile(x+1, y-1, NavLayer);
                        FPaperTileInfo leftTile = TileMapComponent->GetTile(x-1, y, NavLayer);
                        FPaperTileInfo rightTile = TileMapComponent->GetTile(x+1, y, NavLayer);
                        if (upTile.PackedTileIndex == INDEX_NONE) {
                            /*
                            var wp:WayPoint = new WayPoint(x*16+4, (y-1)*16+4, wpCount, "awp"+wpCount, state);
                            waypoints.push(wp);
                            wpCount++;
                             */
                            WayPoint wp = WayPoint();
                            wp.Location = FVector(x * 16,
                                                  0,
                                                  ((MapHeight-1)-y+1)*16 -((MapHeight-1)*16 - actor->GetActorLocation().Z));
                            WayPoints.Add(wp);
                            UE_LOG(LogTemp, Warning, TEXT("WP %s"), *wp.Location.ToString());
                        }
                        /*
                        if (upLeftTile.PackedTileIndex == INDEX_NONE
                            && leftTile.PackedTileIndex == INDEX_NONE
                            && upTile.PackedTileIndex == INDEX_NONE) {
                            WayPoint wp = WayPoint();
                            wp.Location = FVector((x-1) * 16,
                                                  0,
                                                  ((MapHeight-1)-y+1)*16 -((MapHeight-1)*16 - actor->GetActorLocation().Z));
                            WayPoints.Add(wp);
                            UE_LOG(LogTemp, Warning, TEXT("WP %s"), *wp.Location.ToString());
                        }
                        if (upRightTile.PackedTileIndex == INDEX_NONE
                            && rightTile .PackedTileIndex == INDEX_NONE
                            && upTile .PackedTileIndex == INDEX_NONE) {
                            WayPoint wp = WayPoint();
                            wp.Location = FVector((x+1) * 16,
                                                  0,
                                                  ((MapHeight-1)-y+1)*16 -((MapHeight-1)*16 - actor->GetActorLocation().Z));
                            WayPoints.Add(wp);
                            UE_LOG(LogTemp, Warning, TEXT("WP %s"), *wp.Location.ToString());
                        }
                        */

                    }
                }
            }

            // Create the links
            for (auto& wp1 : WayPoints) {
                for (auto& wp2 : WayPoints) {
                    if (wp1 != wp2) {
                        //trace(wp1.x + ","+wp1.y+" - " + wp2.x+","+wp2.y);
                        FHitResult hit;
                        bool traceRes = GetWorld()->LineTraceSingleByChannel(hit, wp1.Location, wp2.Location, ECC_WorldStatic);
                        if (FVector::Dist(wp1.Location, wp2.Location) < 16*5 /* && !traceRes */) {
                            Link link = Link();
                            link.Start = &wp1;
                            link.End = &wp2;
                            Links.Add(link);

                            //FVector spawnLocation = FVector(link.Start->Location.X, 0, link.Start->Location.Z);
                            FVector spawnLocation = FVector(0, 0, 0);
                            FRotator spawnRotation(0.0f, -90.0f, 0.0f);
                            FActorSpawnParameters spawnParameters;
                            ANavLinkProxy *navLinkProxy = GetWorld()->SpawnActor<ANavLinkProxy>(spawnLocation, spawnRotation, spawnParameters);
                            //FNavigationLink point = navLinkProxy->PointLinks[0];
                            navLinkProxy->PointLinks[0].Left = FVector(0, link.Start->Location.X, link.Start->Location.Z);
                            navLinkProxy->PointLinks[0].Right = FVector(0, link.End->Location.X, link.End->Location.Z);
                            navLinkProxy->PointLinks[0].SnapRadius = 8.0f;
                            navLinkProxy->PointLinks[0].SnapHeight = 8.0f;
                            navLinkProxy->PointLinks[0].bUseSnapHeight = true;
                            navLinkProxy->PointLinks[0].SetAreaClass(UNavArea_Jump::StaticClass());
                            UNavLinkCustomComponent* smartLink = navLinkProxy->GetSmartLinkComp();
                            smartLink->SetEnabled(false);
                        }
                    }
                }
            }


        } else {
            UE_LOG(LogTemp, Warning, TEXT("TilemapNavComponent: actor has no TileMapComponent."));
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("TilemapNavComponent has no owner."));
    }

    // ...
	
}


// Called every frame
void UTilemapNavComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

