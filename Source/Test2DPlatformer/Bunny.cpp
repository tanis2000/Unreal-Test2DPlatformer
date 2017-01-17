// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "Bunny.h"
#include "PaperSpriteComponent.h"
#include "PaperSpriteAtlas.h"

FName ABunny::SpriteComponentName(TEXT("Sprite0"));

// Sets default values
ABunny::ABunny()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    
    // Setup the assets
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UPaperSprite> IdleSpriteAsset;
        FConstructorStatics()
        : IdleSpriteAsset(TEXT("/Game/Sprites/HeroSpriteNoColl"))
        {
        }
    };
    static FConstructorStatics ConstructorStatics;
    
    IdleSprite = ConstructorStatics.IdleSpriteAsset.Get();

    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    UE_LOG(LogTemp, Warning, TEXT("Bunny"));
  
  // Uncomment to disable physics
  //this->DisableComponentsSimulatePhysics();
    
    // Try to create the sprite component
    Sprite = CreateOptionalDefaultSubobject<UPaperSpriteComponent>(ABunny::SpriteComponentName);
    if (Sprite)
    {
        Sprite->AlwaysLoadOnClient = true;
        Sprite->AlwaysLoadOnServer = true;
        Sprite->bOwnerNoSee = false;
        Sprite->bAffectDynamicIndirectLighting = true;
        Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
      Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        //static FName CollisionProfileName(TEXT("CharacterMesh"));
      static FName CollisionProfileName(TEXT("OverlapAll"));
        Sprite->SetCollisionProfileName(CollisionProfileName);
        Sprite->bGenerateOverlapEvents = true;
      Sprite->bAutoActivate = true;
      // TODO: maybe not needed
      //Sprite->bCanEverAffectNavigation = false;
      
        // Enable replication on the Sprite component so animations show up when networked
        Sprite->SetIsReplicated(true);
        Sprite->SetSprite(IdleSprite);
      
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Problem creating sprite"));
    }

  // enables hit events
  SetActorEnableCollision(true);
    SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
  
  OnActorHit.AddDynamic(this, &ABunny::OnMyActorHit);
    Sprite->OnComponentBeginOverlap.AddDynamic(this, &ABunny::OnBeginOverlap);
    Sprite->OnComponentEndOverlap.AddDynamic(this, &ABunny::OnEndOverlap);
  bReplicates = true;
    
    FVector v = GetActorLocation();
    posX = v.X;
    posY = v.Z;
    speedX = FMath::FRandRange(0, 1) * 5;
    speedY = FMath::FRandRange(0, 1) * 5 - 2.5f;

}

// Called when the game starts or when spawned
void ABunny::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABunny::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    posX += speedX;
    posY += speedY;
    speedY += gravity;
    
    if( posX > maxX )
    {
        speedX *= -1;
        posX = maxX;
    }
    else if( posX < minX )
    {
        speedX *= -1;
        posX = minX;
    }
    
    if( posY > maxY )
    {
        speedY *= -0.8f;
        posY = maxY;
        if( FMath::FRandRange(0, 1) > 0.5f )
            speedY -= 3 + FMath::FRandRange(0, 1) * 4;
    }
    else if( posY < minY )
    {
        speedY = 0;
        posY = minY;
    }
    newPos.X = posX;
    newPos.Y = 0;
    newPos.Z = posY;
    
    SetActorLocation(newPos);
}

// Called to bind functionality to input
void ABunny::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ABunny::ReceiveHit(class UPrimitiveComponent *MyComp, AActor *Other, class UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
  UE_LOG(LogTemp, Warning, TEXT("ReceiveHit"));
}

void ABunny::OnMyActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
  UE_LOG(LogTemp, Warning, TEXT("OnActorHit"));
  
}

void ABunny::OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    //UE_LOG(LogTemp, Warning, TEXT("OnBeginOverlap"));
    speedX *= -1;
    speedY *= -1;
    posX = SweepResult.Normal.X * SweepResult.PenetrationDepth;
    posY = SweepResult.Normal.Y * SweepResult.PenetrationDepth;
    newPos.X = posX;
    newPos.Y = 0;
    newPos.Z = posY;
    
    SetActorLocation(newPos);
}


void ABunny::OnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    //UE_LOG(LogTemp, Warning, TEXT("OnEndOverlap"));
    /*if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
    {
        // Only add impulse and destroy projectile if we hit a physics object
        if (OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(MyVelocity * 100.f, GetActorLocation());
            
            Destroy();
        }
    }*/
}

