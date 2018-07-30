// Fill out your copyright notice in the Description page of Project Settings.

#include "BunnyManager.h"
#include "Test2DPlatformer.h"
#include "HeroPawn.h"
#include "Bunny.h"
#include "PaperGroupedSpriteComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/World.h"
#include "Engine.h"

FName ABunnyManager::GroupComponentName(TEXT("BunnyGroup"));

// Sets default values
ABunnyManager::ABunnyManager() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    group = CreateOptionalDefaultSubobject<UPaperGroupedSpriteComponent>(ABunnyManager::GroupComponentName);

}

// Called when the game starts or when spawned
void ABunnyManager::BeginPlay() {
    Super::BeginPlay();

}

// Called every frame
void ABunnyManager::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    while (bunniesCount < NumberOfBunniesToSpawn) {
        FVector Location = FVector(20, 0, 40);
        ABunny *bunny = (ABunny *) GetWorld()->SpawnActor(ABunny::StaticClass(), &Location);
        //group->AddInstance(bunny->GetTransform(), bunny->IdleSprite);
        bunniesCount++;
    }

    /*
      int32 i = 0;
      for (TActorIterator<ABunny> ActorItr(GetWorld()); ActorItr; ++ActorItr)
      {
          FTransform t = ActorItr->GetTransform();
          group->UpdateInstanceTransform(i, t);
          i++;
      }
  */
}

void ABunnyManager::AddBunnies(int num) {
    for (int i = 0; i < num; i++) {
        FVector Location = FVector(20, 0, 40);
        ABunny *bunny = (ABunny *) GetWorld()->SpawnActor(ABunny::StaticClass(), &Location);
        bunniesCount++;
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bunnies: %d"), bunniesCount));
}

