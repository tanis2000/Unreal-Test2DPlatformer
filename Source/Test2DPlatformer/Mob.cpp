// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Test2DPlatformer.h"

FName AMob::SpriteComponentName(TEXT("MainSprite"));

// Sets default values
AMob::AMob() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  // Setup the assets
  struct FConstructorStatics {
    ConstructorHelpers::FObjectFinderOptional<UPaperSprite> MainSpriteAsset;
    FConstructorStatics() : MainSpriteAsset(TEXT("/Game/Sprites/HeroSprite")) {}
  };
  static FConstructorStatics ConstructorStatics;

  MainSprite = ConstructorStatics.MainSpriteAsset.Get();

  // Create a dummy root component we can attach things to.
  RootComponent =
      CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

  SpriteComponent = CreateOptionalDefaultSubobject<UPaperSpriteComponent>(
      AMob::SpriteComponentName);
  if (SpriteComponent) {
    SpriteComponent->AlwaysLoadOnClient = true;
    SpriteComponent->AlwaysLoadOnServer = true;
    SpriteComponent->bOwnerNoSee = false;
    SpriteComponent->bAffectDynamicIndirectLighting = true;
    SpriteComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    SpriteComponent->AttachToComponent(
        RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    static FName CollisionProfileName(TEXT("OverlapAll"));
    SpriteComponent->SetCollisionProfileName(CollisionProfileName);
    SpriteComponent->SetGenerateOverlapEvents(true);
    SpriteComponent->bAutoActivate = true;
    // TODO: maybe not needed
    // SpriteComponent->bCanEverAffectNavigation = false;

    // Enable replication on the Sprite component so animations show up when
    // networked
    SpriteComponent->SetIsReplicated(true);
    SpriteComponent->SetSprite(MainSprite);

  } else {
    UE_LOG(LogTemp, Warning, TEXT("Problem creating sprite"));
  }

  // enables hit events
  SetActorEnableCollision(true);
  SpawnCollisionHandlingMethod =
      ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

  SpriteComponent->OnComponentBeginOverlap.AddDynamic(this,
                                                      &AMob::OnBeginOverlap);
  SpriteComponent->OnComponentEndOverlap.AddDynamic(this, &AMob::OnEndOverlap);
}

// Called when the game starts or when spawned
void AMob::BeginPlay() {
  Super::BeginPlay();

  APlayerController *PCOwner = Cast<APlayerController>(GetOwner());

  if (PCOwner) {
    InputComponent =
        CreateDefaultSubobject<UInputComponent>(TEXT("InputComponent"));
    InputComponent->BindAxis("MoveRight", this, &AMob::MoveRight);
    EnableInput(PCOwner);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("No owner"));
  }
}

// Called every frame
void AMob::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  FVector StartLocation = GetActorLocation();
  FVector EndLocation =
      FVector(StartLocation.X, StartLocation.Y, StartLocation.Z);
  // EndLocation.Z -= 10;

  FVector bounds = FVector(8, 8, 8);
  FCollisionShape box = FCollisionShape::MakeBox(bounds);
  //FCollisionQueryParams params = FCollisionQueryParams::FCollisionQueryParams();

  FHitResult hitRes;

  GetWorld()->SweepSingleByProfile(hitRes, StartLocation, EndLocation,
                                   FQuat::Identity, TEXT("OverlapAll"), box);

  if (hitRes.GetActor() != nullptr) {
    GEngine->AddOnScreenDebugMessage(
        -1, 5.f, FColor::Red, FString::Printf(TEXT("Time: %f"), hitRes.Time));
    return;
  }
  SetActorLocation(EndLocation);
}

void AMob::MoveRight(float Value) {
  FVector Location = GetActorLocation();
  Location.X += 10;
  SetActorLocation(Location);
}

void AMob::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent,
                          AActor *OtherActor, UPrimitiveComponent *OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep,
                          const FHitResult &SweepResult) {
  if (OtherActor != this) {
    UE_LOG(LogTemp, Warning, TEXT("OnBeginOverlap"));
    FVector Location = GetActorLocation();

    if (!SweepResult.bStartPenetrating) {
      GEngine->AddOnScreenDebugMessage(
          -1, 5.f, FColor::Red,
          FString::Printf(TEXT("Distance: %f"), SweepResult.Distance));
      GEngine->AddOnScreenDebugMessage(
          -1, 5.f, FColor::Red,
          FString::Printf(TEXT("Pene: %f"), SweepResult.PenetrationDepth));
      GEngine->AddOnScreenDebugMessage(
          -1, 5.f, FColor::Red,
          FString::Printf(TEXT("Loc: %f %f %f"), SweepResult.Location.X,
                          SweepResult.Location.Y, SweepResult.Location.Z));
      Location.Z += SweepResult.Distance;
      GEngine->AddOnScreenDebugMessage(
          -1, 5.f, FColor::Red,
          FString::Printf(TEXT("Imp: %f %f %f"), SweepResult.ImpactNormal.X,
                          SweepResult.ImpactNormal.Y,
                          SweepResult.ImpactNormal.Z));
      Location.Z += SweepResult.Distance;
    } else {
      GEngine->AddOnScreenDebugMessage(
          -1, 5.f, FColor::Red,
          FString::Printf(TEXT("Pene: %f"), SweepResult.PenetrationDepth));
      Location.Z += SweepResult.PenetrationDepth;
    }
    SetActorLocation(Location);
  }
}

void AMob::OnEndOverlap(UPrimitiveComponent *OverlappedComponent,
                        AActor *OtherActor, UPrimitiveComponent *OtherComp,
                        int32 OtherBodyIndex) {
  // UE_LOG(LogTemp, Warning, TEXT("OnEndOverlap"));
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
