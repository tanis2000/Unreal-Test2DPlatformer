// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "HeroPawn.h"
#include "PaperFlipbookComponent.h"
#include "BunnyManager.h"

FName AHeroPawn::SpriteComponentName(TEXT("Sprite0"));


// Sets default values
AHeroPawn::AHeroPawn()
{
    // Setup the assets
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
        FConstructorStatics()
        : IdleAnimationAsset(TEXT("/Game/Animations/HeroIdle.HeroIdle"))
        {
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
    if (Sprite)
    {
        Sprite->AlwaysLoadOnClient = true;
        Sprite->AlwaysLoadOnServer = true;
        Sprite->bOwnerNoSee = false;
        Sprite->bAffectDynamicIndirectLighting = true;
        Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        Sprite->AttachParent = RootComponent;
        static FName CollisionProfileName(TEXT("CharacterMesh"));
        Sprite->SetCollisionProfileName(CollisionProfileName);
        Sprite->bGenerateOverlapEvents = false;
        
        Sprite->SetFlipbook(IdleAnimation);

        // Enable replication on the Sprite component so animations show up when networked
        Sprite->SetIsReplicated(true);

    }

    // Set this pawn to be controlled by the lowest-numbered player
    AutoPossessPlayer = EAutoReceiveInput::Player0;
    
    // Create an orthographic camera (no perspective) and attach it to the boom
    SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->OrthoWidth = 512.0f;
    SideViewCameraComponent->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
    SideViewCameraComponent->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
    SideViewCameraComponent->AttachTo(RootComponent);
    
    // Prevent all automatic rotation behavior on the camera, character, and camera component
    SideViewCameraComponent->bUsePawnControlRotation = false;

    
    bReplicates = true;


}

// Called when the game starts or when spawned
void AHeroPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeroPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
    
    if (!CurrentVelocity.IsZero())
    {
        FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
        SetActorLocation(NewLocation);
    }

    TArray<AActor*> actors;
    Sprite->GetOverlappingActors(actors, NULL);
    // then merge it into our final list
    for (auto CompIt = actors.CreateIterator(); CompIt; ++CompIt)
    {
        AActor* OverlappingActor = *CompIt;
        if(OverlappingActor != this)
        {
            CurrentVelocity.Z = 100;
        }
    }

}

// Called to bind functionality to input
void AHeroPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAxis("MoveRight", this, &AHeroPawn::MoveRight);


}

void AHeroPawn::MoveRight(float Value)
{
    // Move at 100 units per second forward or backward
    CurrentVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * 100.0f;
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Value: %f"), Value));

    if (Value > 0.5f) {
        UE_LOG(LogTemp, Warning, TEXT("MOVE"));
        for (TActorIterator<ABunnyManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
        {
            ActorItr->AddBunnies(1000);
        }
    }
    
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

