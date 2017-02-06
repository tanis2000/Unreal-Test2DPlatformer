// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TEST2DPLATFORMER_API ABasePawn : public APawn
{
	GENERATED_BODY()

protected:
	FVector SubPixelCounter;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent *WorldCollisionBoxComponent;

    bool IgnoreJumpThrus;

public:

    float TimeRate = 1.0f;
    float LastTimeMult = 0.0f;
    float ActualDeltaTime = 0.0f;
    float TimeMult = 0.0f;
    float ComputedDeltaTime = 0.0f;

	// Sets default values for this pawn's properties
	ABasePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	bool MoveH(float moveH);

	bool MoveV(float moveV);

	AActor *CollideFirst(float x, float z);

    bool CollideCheck(AActor *other, float x, float z);

    static float Approach(float start, float end, float shift);

    static FVector Approach(FVector val, FVector target, float maxMove);

    void Move(FVector amount);

    void MoveTowards(FVector target, float maxAmount);

    FVector ActualPosition();

};
