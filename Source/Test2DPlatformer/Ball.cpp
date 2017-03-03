// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "Ball.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "HeroPawn.h"

FName ABall::SpriteComponentName(TEXT("Sprite0"));

ABall::ABall()
{
    // Setup the assets
    struct FConstructorStatics {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;

        FConstructorStatics()
                : IdleAnimationAsset(TEXT("/Game/Animations/BallIdle.BallIdle")) {
        }
    };
    static FConstructorStatics ConstructorStatics;

    IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();

    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    UE_LOG(LogTemp, Warning, TEXT("TEST"));

    // Try to create the sprite component
    Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(ABall::SpriteComponentName);
    if (Sprite) {
        Sprite->AlwaysLoadOnClient = true;
        Sprite->AlwaysLoadOnServer = true;
        Sprite->bOwnerNoSee = false;
        Sprite->bAffectDynamicIndirectLighting = true;
        Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        static FName CollisionProfileName(TEXT("OverlapAllDynamic"));
        Sprite->SetCollisionProfileName(CollisionProfileName);
        Sprite->bGenerateOverlapEvents = false;

        Sprite->SetFlipbook(IdleAnimation);

        // Enable replication on the Sprite component so animations show up when networked
        Sprite->SetIsReplicated(true);

        // Grab the socket
        UPaperFlipbook *flipBook = Sprite->GetFlipbook();
        UPaperSprite *sprite = flipBook->GetSpriteAtFrame(0); // this flipbook is only one frame long
        FPaperSpriteSocket *hitboxSocket = sprite->FindSocket(TEXT("HitboxSocket"));

        // World collision
        WorldCollisionBoxComponent = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("WorldCollisionBoxComponent"));
        WorldCollisionBoxComponent->SetRelativeTransform(hitboxSocket->LocalTransform);
        WorldCollisionBoxComponent->SetCollisionProfileName(CollisionProfileName);
        WorldCollisionBoxComponent->bGenerateOverlapEvents = true;
        FVector boxExtent = FVector(3.8f, 3.8f, 3.8f); // 0.2f is a skin around the sprite
        WorldCollisionBoxComponent->InitBoxExtent(boxExtent);
        WorldCollisionBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    }

    Tags.Add(TEXT("Ball"));

    distanceFromCarrier = 16;

    bReplicates = true;    
}

// Called every frame
void ABall::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (Carrier != nullptr) {
        // If we have a carrier we move with him
        FVector location = Carrier->GetActorLocation();
        location.X += Carrier->Facing * distanceFromCarrier;
        targetLocation = location;
        SetActorLocation(Approach(GetActorLocation(), targetLocation, idleSpeed * TimeMult));
    } else {
        // Temporary vars
        float tempAccel, tempFric;

        // Apply the correct form of acceleration and friction
        if (BottomCollided) {
            tempAccel = groundAccel;
            tempFric  = groundFric;
        } else {
            tempAccel = airAccel;
            tempFric  = airFric;
        }

        // Handle gravity
        if (!BottomCollided) {
            // Fall normally
            Velocity.Z = Approach(Velocity.Z, -vyMax, -gravNorm);
        }

        Velocity.X = Approach(Velocity.X, 0, tempFric);


        MoveH(Velocity.X * DeltaTime * 50);
        MoveV(Velocity.Z * DeltaTime * 50);

        BottomCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X, GetActorLocation().Z-1) != nullptr;
    }


    AHeroPawn *hero = HeroCollides(GetActorLocation().X, GetActorLocation().Z);
    if (hero != nullptr) {
        Carrier = hero;
        UE_LOG(LogTemp, Warning, TEXT("Set new carrier"));
    }
}

AHeroPawn *ABall::HeroCollides(float x, float z) {
    AHeroPawn *res = nullptr;
    FVector originalLocation = GetActorLocation();
    FVector newLocation = GetActorLocation();
    newLocation.X = x;
    newLocation.Z = z;
    SetActorLocation(newLocation);

    TArray<AActor *> actors;
    WorldCollisionBoxComponent->GetOverlappingActors(actors, AHeroPawn::StaticClass());
    for (auto CompIt = actors.CreateIterator(); CompIt; ++CompIt) {
        //UE_LOG(LogTemp, Warning, TEXT("Overlapping actor."));
        AActor *OverlappingActor = *CompIt;
        if (OverlappingActor != this) {
            UE_LOG(LogTemp, Warning, TEXT("CollideCheck: Actor: %s"), *AActor::GetDebugName(OverlappingActor));
            res = Cast<AHeroPawn>(OverlappingActor);
        }
    }

    SetActorLocation(originalLocation);
    return res;
}

void ABall::Shoot(float speed)
{
    Carrier = nullptr;
    Velocity.X = speed;
}


