// Fill out your copyright notice in the Description page of Project Settings.

#include "Goal.h"
#include "Test2DPlatformer.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AGoal::AGoal()
{
    // Setup the assets
    struct FConstructorStatics {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
        ConstructorHelpers::FObjectFinderOptional<UParticleSystem> GoalParticleSystemAsset;

        FConstructorStatics()
                : 
                IdleAnimationAsset(TEXT("/Game/Animations/HeroIdle.HeroIdle")),
                GoalParticleSystemAsset(TEXT("/Game/Particles/GoalEmitter.GoalEmitter"))
                 {}
    };
    static FConstructorStatics ConstructorStatics;

    IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Try to create the sprite component
    Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
    if (Sprite) {
        Sprite->AlwaysLoadOnClient = true;
        Sprite->AlwaysLoadOnServer = true;
        Sprite->bOwnerNoSee = false;
        Sprite->bAffectDynamicIndirectLighting = true;
        Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        static FName CollisionProfileName(TEXT("OverlapAllDynamic"));
        Sprite->SetCollisionProfileName(CollisionProfileName);
		Sprite->SetGenerateOverlapEvents(false);

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
		WorldCollisionBoxComponent->SetGenerateOverlapEvents(true);
        FVector boxExtent = FVector(7.8f, 7.8f, 7.8f); // 0.2f is a skin around the sprite
        WorldCollisionBoxComponent->InitBoxExtent(boxExtent);
        WorldCollisionBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

        GoalParticleSystem = ConstructorStatics.GoalParticleSystemAsset.Get();
        GoalEmitter = CreateOptionalDefaultSubobject<UParticleSystemComponent>(TEXT("GoalEmitter"));
        GoalEmitter->SetTemplate(GoalParticleSystem);
        GoalEmitter->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

        /*
        ConstructorHelpers::FObjectFinder<UParticleSystem> ArbitraryParticleName(TEXT("ParticleSystem'/Game/Particles/GoalEmitter.GoalEmitter'"));
        if (ArbitraryParticleName.Succeeded()) {
            PuffEmitter = ArbitraryParticleName.Object;
            if (PuffEmitter != nullptr) {
                UGameplayStatics::SpawnEmitterAttached(PuffEmitter, Sprite);
            }
        }
        */

    }

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

