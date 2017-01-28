// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "HeroPawn.h"
#include "PaperFlipbookComponent.h"
#include "BunnyManager.h"
#include "PixelPerfectCameraComponent.h"

FName AHeroPawn::SpriteComponentName(TEXT("Sprite0"));


// Sets default values
AHeroPawn::AHeroPawn() {
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
    Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AHeroPawn::SpriteComponentName);
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

        // Hitbox
        HitBoxComponent = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("HitBoxComponent"));
        HitBoxComponent->SetRelativeTransform(hitboxSocket->LocalTransform);
        HitBoxComponent->SetCollisionProfileName(CollisionProfileName);
        HitBoxComponent->bGenerateOverlapEvents = true;
        FVector hitboxExtent = FVector(3.8f, 3.8f, 3.8f); // 0.2f is a skin around the sprite
        HitBoxComponent->InitBoxExtent(hitboxExtent);
        HitBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    }

    // Set this pawn to be controlled by the lowest-numbered player
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    // Create an orthographic camera (no perspective) and attach it to the boom
    SideViewCameraComponent = CreateDefaultSubobject<UPixelPerfectCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->SetAspectRatio(1.333334f);
    SideViewCameraComponent->OrthoWidth = 320.0f;
    SideViewCameraComponent->bConstrainAspectRatio = true;
    SideViewCameraComponent->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
    SideViewCameraComponent->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
    SideViewCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    // Prevent all automatic rotation behavior on the camera, character, and camera component
    SideViewCameraComponent->bUsePawnControlRotation = false;

    // Read the viewport size
    FVector2D viewportSize = FVector2D(1, 1);
    if (GEngine && GEngine->GameViewport) {
        GEngine->GameViewport->GetViewportSize(viewportSize);
        UE_LOG(LogTemp, Warning, TEXT("VX: %f"), viewportSize.X);
        UE_LOG(LogTemp, Warning, TEXT("VY: %f"), viewportSize.Y);
        SideViewCameraComponent->SetAspectRatio(viewportSize.X/viewportSize.Y);
        SideViewCameraComponent->OrthoWidth = viewportSize.X;
    }

    // Read the resolution
    FVector2D resolution = FVector2D( 1, 1 );
    resolution.X = GSystemResolution.ResX;
    resolution.Y = GSystemResolution.ResY;
    UE_LOG(LogTemp, Warning, TEXT("RX: %f"), resolution.X);
    UE_LOG(LogTemp, Warning, TEXT("RY: %f"), resolution.Y);
    SideViewCameraComponent->SetAspectRatio(resolution.X/resolution.Y);
    SideViewCameraComponent->OrthoWidth = resolution.X;

    SideViewCameraComponent->BestFit();

    bReplicates = true;


}

// Called when the game starts or when spawned
void AHeroPawn::BeginPlay() {
    Super::BeginPlay();

}

// Called every frame
void AHeroPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (!CurrentVelocity.IsZero()) {
        FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
        SetActorLocation(NewLocation);
    }

    TArray<AActor *> actors;
    Sprite->GetOverlappingActors(actors, NULL);
    // then merge it into our final list
    for (auto CompIt = actors.CreateIterator(); CompIt; ++CompIt) {
        UE_LOG(LogTemp, Warning, TEXT("Overlapping actor."));
        AActor *OverlappingActor = *CompIt;
        if (OverlappingActor != this) {
            UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *AActor::GetDebugName(OverlappingActor));
            //CurrentVelocity.Z = 100;
        }
    }

}

// Called to bind functionality to input
void AHeroPawn::SetupPlayerInputComponent(class UInputComponent *InputComponent) {
    Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAxis("MoveSide", this, &AHeroPawn::MoveSide);
    InputComponent->BindAxis("MoveVertical", this, &AHeroPawn::MoveVertical);
    InputComponent->BindAction("SpawnBunny", EInputEvent::IE_Released, this, &AHeroPawn::SpawnBunnies);
}

void AHeroPawn::MoveSide(float Value) {
    MoveH(Value);

/*
    // Move at 100 units per second forward or backward
    CurrentVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * 100.0f;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Value: %f"), Value));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("VX: %f"), CurrentVelocity.X));

    if (Value > 0.0f) {
        UE_LOG(LogTemp, Warning, TEXT("MOVE RIGHT"));
    } else if (Value < 0.0f) {
        UE_LOG(LogTemp, Warning, TEXT("MOVE LEFT"));
    }
*/
    // Update animation to match the motion
    //UpdateAnimation();

    // Set the rotation so that the character faces his direction of travel.
    /*
    if (Controller != nullptr)
    {
        if (Value < 0.0f)
        {
            Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
        }
        else if (Value > 0.0f)
        {
            Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
        }
    }
    
    // Apply the input to the character motion
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
     */
}

void AHeroPawn::MoveVertical(float Value) {
    MoveV(Value);
}

void AHeroPawn::SpawnBunnies() {
    for (TActorIterator<ABunnyManager> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
        ActorItr->AddBunnies(1000);
    }
}

void AHeroPawn::NotifyActorBeginOverlap(AActor *OtherActor) {
    AActor::NotifyActorBeginOverlap(OtherActor);
    UE_LOG(LogTemp, Warning, TEXT("BEGIN OVERLAP"));
}

bool AHeroPawn::MoveH(float moveH) {
    SubPixelCounter.X += moveH;
    int32 dx = FMath::RoundToInt(SubPixelCounter.X);
    if (dx != 0) {
        int8 sign = FMath::Sign(dx);
        SubPixelCounter.X -= sign;
        while (dx != 0) {
            AActor *entity = CollideFirst(/*GameTags.Solid, */GetActorLocation().X + sign, GetActorLocation().Z);
            if (entity != NULL)
            {
                SubPixelCounter.X = 0.0f;
                /*if (onCollide != null)
                {
                    onCollide(entity as Platform);
                }*/
                return true;
            }
            FVector location = GetActorLocation();
            location.X += sign;
            SetActorLocation(location);
            dx -= sign;
        }
    }
    return false;
}

bool AHeroPawn::MoveV(float moveV) {
    AActor *entity;
    SubPixelCounter.Z += moveV;
    int32 dy = FMath::RoundToInt(SubPixelCounter.Z);
    if (dy < 0)
    {
        SubPixelCounter.Z -= dy;
        while (dy != 0)
        {
            entity = CollideFirst(/*GameTags.Solid, */GetActorLocation().X, GetActorLocation().Z - 1.0f);
            if (entity != nullptr)
            {
                SubPixelCounter.Z = 0.0f;
                /*if (onCollide != null)
                {
                    onCollide(entity as Platform);
                }*/
                return true;
            }
            FVector location = GetActorLocation();
            location.Z += -1;
            SetActorLocation(location);
            dy -= -1;
        }
    }
    else if (dy > 0)
    {
        SubPixelCounter.Z -= dy;
        while (dy != 0)
        {
            entity = CollideFirst(/*GameTags.Solid, */GetActorLocation().X, GetActorLocation().Z + 1.0f);
            if (entity != nullptr)
            {
                SubPixelCounter.Z = 0.0f;
                /*if (onCollide != null)
                {
                    onCollide(entity as Platform);
                }*/
                return true;
            }
            if (!IgnoreJumpThrus && ((entity = CollideFirst(/*GameTags.JumpThru, */GetActorLocation().X, GetActorLocation().Z + 1.0f)) != nullptr))
            {
                SubPixelCounter.Z = 0.0f;
                /*if (onCollide != null)
                {
                    onCollide(entity as Platform);
                }*/
                return true;
            }
            FVector location = GetActorLocation();
            location.Z += 1.0f;
            SetActorLocation(location);
            dy--;
        }
    }
    return false;
}

AActor *AHeroPawn::CollideFirst(float x, float z) {
    AActor *res = NULL;
    FVector originalLocation = GetActorLocation();
    FVector newLocation = GetActorLocation();
    newLocation.X = x;
    newLocation.Z = z;
    SetActorLocation(newLocation);

    TArray<AActor *> actors;
    UPaperFlipbook *flipBook = Sprite->GetFlipbook();
    UPaperSprite *sprite = flipBook->GetSpriteAtTime(Sprite->GetPlaybackPosition(), false);
    FPaperSpriteSocket *hitboxSocket = sprite->FindSocket(TEXT("HitboxSocket"));
    // TODO: we should have set up a child component attached to the socket of every sprite making up the flipbook
    // and now we could decide to query using that one instead of the main collision.
    WorldCollisionBoxComponent->GetOverlappingActors(actors, NULL);
    for (auto CompIt = actors.CreateIterator(); CompIt; ++CompIt) {
        UE_LOG(LogTemp, Warning, TEXT("Overlapping actor."));
        AActor *OverlappingActor = *CompIt;
        if (OverlappingActor != this) {
            UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *AActor::GetDebugName(OverlappingActor));
            res = OverlappingActor;
        }
    }

    SetActorLocation(originalLocation);
    return res;
}