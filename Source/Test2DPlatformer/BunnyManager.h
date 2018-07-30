// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BunnyManager.generated.h"

UCLASS()

class TEST2DPLATFORMER_API ABunnyManager : public AActor {
    GENERATED_BODY()

public:

    UPROPERTY(Category = Manager, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    int32 NumberOfBunniesToSpawn = 0;

    // Sets default values for this actor's properties
    ABunnyManager();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    int32 bunniesCount = 0;

    void AddBunnies(int num);

    class UPaperGroupedSpriteComponent *group;

    static FName GroupComponentName;

};
