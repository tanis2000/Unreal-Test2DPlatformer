// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "PaperSprite.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"
#include "Test2DPlatformer/HeroPawn.h"

FName ABullet::SpriteComponentName(TEXT("Sprite0"));

// Sets default values
ABullet::ABullet(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
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
    UE_LOG(LogTemp, Warning, TEXT("Created Bullet RootComponent"));

    // Try to create the sprite component
    Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(ABullet::SpriteComponentName);
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
        FVector boxExtent = FVector(3.8f, 3.8f, 3.8f); // 0.2f is a skin around the sprite
        WorldCollisionBoxComponent->InitBoxExtent(boxExtent);
        WorldCollisionBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    }

    Tags.Add(TEXT("Bullet"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Temporary vars
	//        float tempAccel;
	float tempFric;

	// Apply the correct form of acceleration and friction
	if (BottomCollided) {
		//            tempAccel = groundAccel;
		tempFric  = groundFric;
	} else {
		//            tempAccel = airAccel;
		tempFric  = airFric;
	}

	// Check collisions with walls and adjust velocity
	if (LeftCollided || RightCollided && Velocity.X != 0) {
		Velocity.X *= -1.0f;
		Velocity.X *= bounceFric;
		UE_LOG(LogTemp, Warning, TEXT("Bullet bounce"));
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
	LeftCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X-1, GetActorLocation().Z) != nullptr;
	TopCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X, GetActorLocation().Z+1) != nullptr;
	RightCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X+1, GetActorLocation().Z) != nullptr;

	CheckHeroCollisions(GetActorLocation().X, GetActorLocation().Z);
}

// Called to bind functionality to input
void ABullet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABullet::Shoot(float speed)
{
	Velocity.X = speed;
}

 void ABullet::CheckHeroCollisions(float x, float z) {
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
			UE_LOG(LogTemp, Warning, TEXT("Bullet CheckHeroCollisions: Actor: %s"), *AActor::GetDebugName(OverlappingActor));
			AHeroPawn *res = Cast<AHeroPawn>(OverlappingActor);
			res->ApplyDamage(10);
			Destroy();
		}
	}
}

void ABullet::OnCollide(AActor *Actor)
{
	if (Actor->IsA(AHeroPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet OnCollide: Actor: %s"), *AActor::GetDebugName(Actor))
		AHeroPawn *res = Cast<AHeroPawn>(Actor);
		res->ApplyDamage(10);
		Destroy();
	}
}