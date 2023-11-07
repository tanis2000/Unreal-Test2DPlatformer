// Fill out your copyright notice in the Description page of Project Settings.

#include "TilemapNavComponent.h"
#include "Test2DPlatformer.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkCustomComponent.h"
#include "NavArea_Jump.h"
#include "Engine/World.h"
#include "Navigation/PlatformerNavLinkProxy.h"


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

	if (!bEnableLinkGeneration)
	{
		return;
	}
	
    AActor *Actor = GetOwner();
    if (Actor != nullptr) {
        TileMapComponent = Actor->FindComponentByClass<UPaperTileMapComponent>();
        if (TileMapComponent != nullptr) {
            TileMapComponent->GetMapSize(MapWidth, MapHeight, NumLayers);
            UE_LOG(LogTemp, Warning, TEXT("Map size, X: %d, Y: %d Layers: %d - Using layer %d"), MapWidth, MapHeight, NumLayers, NavLayer);

            // Build the connection between jumping/fallout positions
            for (int32 y = 0 ; y < MapHeight; y++) {
                for (int32 x = 0 ; x < MapWidth; x++) {
                    FPaperTileInfo Tile = TileMapComponent->GetTile(x, y, NavLayer);
                    int32 TileIndex = Tile.GetTileIndex();
                    int32 PackedTileIndex = Tile.PackedTileIndex;
                    if (PackedTileIndex != INDEX_NONE) {
                        UE_LOG(LogTemp, Log, TEXT("X: %d, Y: %d, TileIndex: %d, PackedTileIndex: %d"), x, y, TileIndex, PackedTileIndex);

                        FPaperTileInfo UpTile = TileMapComponent->GetTile(x, y-1, NavLayer);
                        FPaperTileInfo upLeftTile = TileMapComponent->GetTile(x-1, y-1, NavLayer);
                        FPaperTileInfo upRightTile = TileMapComponent->GetTile(x+1, y-1, NavLayer);
                        FPaperTileInfo leftTile = TileMapComponent->GetTile(x-1, y, NavLayer);
                        FPaperTileInfo rightTile = TileMapComponent->GetTile(x+1, y, NavLayer);
                        if (UpTile.PackedTileIndex == INDEX_NONE) {
                            /*
                            var wp:WayPoint = new WayPoint(x*16+4, (y-1)*16+4, wpCount, "awp"+wpCount, state);
                            waypoints.push(wp);
                            wpCount++;
                             */
                            WayPoint WP = WayPoint();
                            WP.Location = FVector(x * 16,
                                                  0,
                                                  (MapHeight * 16) - (y * 16));
                            WayPoints.Add(WP);
                            UE_LOG(LogTemp, Log, TEXT("WP %s"), *WP.Location.ToString());
                        	// Create the billboard to show the marked areas
                        	if (WayPointClassMarker.Get() != nullptr)
                        	{
                        		FVector Location = WP.Location;
                        		Location.Y = 100;
                        		GetWorld()->SpawnActor(WayPointClassMarker, &Location);
                        	}
                        	
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
                        bool traceRes = GetWorld()->LineTraceSingleByChannel(hit, wp1.Location, wp2.Location, ECC_Visibility);
                        if (FVector::Dist(wp1.Location, wp2.Location) < 16*5  && !traceRes) {
                            Link link = Link();
                            link.Start = &wp1;
                            link.End = &wp2;
                            Links.Add(link);

                            //FVector spawnLocation = FVector(link.Start->Location.X, 0, link.Start->Location.Z);
                        	// This has to be in 0,0,0 because the links are relative
                            FVector spawnLocation = FVector(0, 0, 0);
                            FRotator spawnRotation(0.0f, 0.0f, 0.0f);
                            FActorSpawnParameters spawnParameters;
                        	FString Name = FString::Printf(TEXT("NLP_%ls_%ls"), *link.Start->Location.ToString(), *link.End->Location.ToString());
                        	spawnParameters.Name = "";
                        	spawnParameters.Name.AppendString(Name);
                            APlatformerNavLinkProxy *navLinkProxy = GetWorld()->SpawnActor<APlatformerNavLinkProxy>(spawnLocation, spawnRotation, spawnParameters);
                            //FNavigationLink point = navLinkProxy->PointLinks[0];

                            /*
                            TArray<FNavigationLink> navLinks;
							TArray<FNavigationSegmentLink> navSegLinks;
							navLinkProxy->GetNavigationLinksArray(navLinks, navSegLinks);
							navLinks[0].Left = FVector(0, link.Start->Location.X, link.Start->Location.Z);
							navLinks[0].Right = FVector(0, link.End->Location.X, link.End->Location.Z);
							navLinks[0].SnapRadius = 8.0f;
							navLinks[0].SnapHeight = 8.0f;
							navLinks[0].bUseSnapHeight = true;
							navLinks[0].SetAreaClass(UNavArea_Jump::StaticClass());
                            */
              
							navLinkProxy->PointLinks[0].Left = FVector(link.Start->Location.X, 0, link.Start->Location.Z);
                            navLinkProxy->PointLinks[0].Right = FVector(link.End->Location.X, 0, link.End->Location.Z);
                            navLinkProxy->PointLinks[0].SnapRadius = 8.0f;
                            navLinkProxy->PointLinks[0].SnapHeight = 8.0f;
                            navLinkProxy->PointLinks[0].bUseSnapHeight = true;
                            navLinkProxy->PointLinks[0].SetAreaClass(UNavArea_Jump::StaticClass());

                        	navLinkProxy->SetSmartLinkEnabled(true);
							UNavLinkCustomComponent* smartLink = navLinkProxy->GetSmartLinkComp();
							smartLink->SetEnabled(true);
                        	smartLink->SetLinkData(navLinkProxy->PointLinks[0].Left, navLinkProxy->PointLinks[0].Right, ENavLinkDirection::BothWays);
                        	smartLink->SetEnabledArea(navLinkProxy->PointLinks[0].GetAreaClass());
                        	smartLink->SetNavigationRelevancy(true);
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

