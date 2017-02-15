// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "EnemyPawn.h"
#include "PaperFlipbookComponent.h"
#include "AITypes.h"
#include "PlatformerPawnMovementComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"


FName AEnemyPawn::SpriteComponentName(TEXT("Sprite0"));


// Sets default values
AEnemyPawn::AEnemyPawn() {
    // Setup the assets
    struct FConstructorStatics {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;

        FConstructorStatics()
                : IdleAnimationAsset(TEXT("/Game/Animations/HeroIdle.HeroIdle")) {
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
    Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AEnemyPawn::SpriteComponentName);
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
        FVector boxExtent = FVector(7.8f, 7.8f, 7.8f); // 0.2f is a skin around the sprite
        WorldCollisionBoxComponent->InitBoxExtent(boxExtent);
        WorldCollisionBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

        // AI Movement Component
        MovementComponent = CreateOptionalDefaultSubobject<UPlatformerPawnMovementComponent>(TEXT("PlatformerPawnMovementComponent"));
    }

    bReplicates = true;
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    /*
    if (TargetLocation != FAISystem::InvalidLocation) {
        MoveTowards(TargetLocation, 8.0f * DeltaTime);
    }
     */


    // Handle gravity
    if (!BottomCollided) {
        // Fall normally
        Velocity.Z = Approach(Velocity.Z, -vyMax, -gravNorm);
    }

    MoveV(Velocity.Z * DeltaTime * 50);

    BottomCollided = CollideFirst(GetActorLocation().X, GetActorLocation().Z-1) != nullptr;

}

void AEnemyPawn::Jump(FVector amount)
{
    Velocity.X = amount.X;
    MoveH(Velocity.X);
    if (BottomCollided) {
        Velocity.Z = jumpHeight;
    }
}